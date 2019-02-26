#include <AFMotor.h> //Library for motor control

char BluetoothInputString;
const int trigPin=5;
const int echoPin=6;
const int Red_LED=4;
const int Yellow_LED=7;
const int motorPin = 10;
const int go = 255;
const int reduce = 100;
const int stop = 0;
const int outer_warn = 24;
const int inner_warn = 6;
const int up_close = 2;
int speed = 0;

void setup()  
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);   //This is the ultrasonic sensor used to control the motor, the heart of the obstacle avoidance.
  pinMode(echoPin, INPUT);
  pinMode(Red_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
 
  pinMode(motorPin, OUTPUT); // Set up the motor pin to be an output
    
} // END OF void setup()

void loop() 
{  
  if(Serial.available()>0) {
    BluetoothInputString = Serial.read(); 
    //Change pin mode only if new command is different from previous.   
    //Serial.println(command);
    Serial.println(BluetoothInputString);
    switch (BluetoothInputString) {
      case 'F': {
        ObstacleAvoidance();
        break;
      }
      case 'G': {
        digitalWrite(Red_LED, LOW);
        digitalWrite(Yellow_LED, LOW);
        speed = go;
        break;
      }
      case 'S': {
        digitalWrite(Red_LED, LOW);
        digitalWrite(Yellow_LED, LOW);
        speed = stop;
        break;
      }
    } //end switch
    analogWrite(motorPin,speed);
  }   //endif
}     // END OF void loop()

void ObstacleAvoidance()
{
  long duration, inches, cm;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  
        
  //convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  if (inches > inner_warn && inches <= outer_warn) {//Check if Object is in warning zone
    digitalWrite(Red_LED,LOW); // When the Red condition is met, the Green LED should turn off
    digitalWrite(Yellow_LED,HIGH);
    speed = go - reduce;
    Serial.println(inches+"in, "+cm+"cm");
    return;
  }
  if (inches > up_close && inches <= inner_warn)  {
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Yellow_LED, LOW);
    speed = stop;
    Serial.println(inches+"in, "+cm+"cm");
    return;
  }
  else {
    digitalWrite(Red_LED, LOW);
    digitalWrite(Yellow_LED, LOW
    speed = go;
    Serial.println("Out of range");
  }
  return;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
