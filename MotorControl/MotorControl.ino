/*
  Motor Relay Power Control
  10-30mA for LEDs
  
 */
  
//Motors
  const int Mo5 = 14;
  const int Mo1 = 17;
  const int Mo3 = 18;
//Accelerometer
//  const int Ascl = 82;  //Analog
//  const int Asda = 83;
//  const int Aint = 19;  //Digital
//Relay Power Control
//  const int ReM1 = 23;
//  const int ReM2 = 24;
//  const int ReM3 = 25;
//  const int ReM4 = 26;
//Button
  const int butTon = 2;
  int bState = 0;

void setup() {
Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);
  
  pinMode(Mo1, OUTPUT);
  pinMode(Mo3, OUTPUT);
  pinMode(Mo5, OUTPUT);
/*
  pinMode (Ascl, INPUT);
  pinMode (Asda, INPUT);
  pinMode (Aint, INPUT);

  pinMode (ReM1, OUTPUT);
  pinMode (ReM2, OUTPUT);
  pinMode (ReM3, OUTPUT);
  pinMode (ReM4, OUTPUT);
*/
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(400);
}

void switchForever() {
  digitalWrite(13, LOW);
  digitalWrite(Mo1,HIGH);
  digitalWrite(Mo3,HIGH);
    delay(2000);
    
  digitalWrite(13, HIGH); //light turns on
  digitalWrite(Mo1,LOW);
  digitalWrite(Mo3,LOW);
    delay(1000);
}


void buttonSwitch() {
  bState = digitalRead(butTon);
  digitalWrite(Mo1, bState);
  digitalWrite(Mo3, bState);
  digitalWrite(13, HIGH);
  delay(250);
//  Serial.println("Print\n");
  digitalWrite(13,LOW);
  delay(250);
}

void loop() {

switchForever();
//buttonSwitch();
  
}         //end VOID LOOP
