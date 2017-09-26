/*
  Motor Relay Power Control
  10-30mA for LEDs
  
  This is a demonstration of the Arduino functionality with the digital motors.
 */
  
//Motors
  const int Mo5 = 14;
  const int Mo1 = 17;
  const int Mo3 = 18;

void setup() {
Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);
  
  pinMode(Mo1, OUTPUT);
  pinMode(Mo3, OUTPUT);
  pinMode(Mo5, OUTPUT);

//LEd blink twice, second blink is longer.
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(600);
}

void switchForever() {
  digitalWrite(13, LOW);  //light turns off
  digitalWrite(Mo1,HIGH); //motors spin one way
  digitalWrite(Mo3,HIGH);
    delay(2000);
    
  digitalWrite(13, HIGH); //light turns on
  digitalWrite(Mo1,LOW);  //motors spin opposite
  digitalWrite(Mo3,LOW);
    delay(1000);
}

void loop() {
switchForever(); 
}
