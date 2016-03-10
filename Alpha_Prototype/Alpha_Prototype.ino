#include <AFMotor.h> //Library for motor control

char BluetoothInputString;
const int trigPin=5;
const int echoPin=6;
const int Red_LED=4;
const int Yellow_LED=7;
const int motorPin = 10;
int speed = 0;

void setup() 
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Red_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
 
  pinMode(motorPin, OUTPUT); // Set up the motor pin to be an output
    
} // END OF void setup()-------------------------------------

void loop() 
{  
  if(Serial.available()>0)
  {
    BluetoothInputString = Serial.read(); 
    //Change pin mode only if new command is different from previous.   
    //Serial.println(command);
    Serial.print(BluetoothInputString);
    Serial.println();
    switch (BluetoothInputString)
    {
      case 'F':  
      {
        ObstacleAvoidance();
        break;
      }
       
      case 'G':
      {
        digitalWrite(Red_LED, LOW);
        digitalWrite(Yellow_LED, LOW);
        speed = 255;
        analogWrite(motorPin,speed);
        break;
      }
    
      case 'S':
      {
        digitalWrite(Red_LED, LOW);
        digitalWrite(Yellow_LED, LOW);
        speed = 0;
        analogWrite(motorPin,speed);
        break;
      }
    }
    
  }
}   // END OF void loop()

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
  
  if (inches > 6 && inches <= 24) //Check if Object is in warning zone
  {  
    digitalWrite(Red_LED,LOW); // When the Red condition is met, the Green LED should turn off
    digitalWrite(Yellow_LED,HIGH);
    speed = 150;
    analogWrite(motorPin,speed); 
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    return;
  }
  if (cm > 2 && inches <= 6) 
  {
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Yellow_LED, LOW);
    speed = 0;
    analogWrite(motorPin,speed);
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    return;
  }
  else 
  {
    digitalWrite(Red_LED, LOW);
    digitalWrite(Yellow_LED, LOW);
    Serial.println("Out of range");    
    speed = 255;
    analogWrite(motorPin,speed);
    return;
  }
  delay (500);
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


