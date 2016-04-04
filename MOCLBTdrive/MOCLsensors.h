/*
 * Sensors
 */
 #define sensorSize 12
//Digital Ultrasonic Sensors
//t means transmit, e means echo
const int USt1 = 22;
const int USe1 = 23;
const int USt2 = 24;
const int USe2 = 25;
const int USt3 = 26;
const int USe3 = 27;
const int USt4 = 28;
const int USe4 = 29;
const int USt5 = 30;
const int USe5 = 31;
const int USt6 = 32;
const int USe6 = 33;
const int USt7 = 34;
const int USe7 = 35;
const int USt8 = 36;
const int USe8 = 37;
const int USt9 = 38;
const int USe9 = 39;
const int USt10 = 40;
const int USe10 = 41;
const int USt11 = 42;
const int USe11 = 43;
const int USt12 = 44;
const int USe12 = 45;
int transmitters[] = {USt1, USt2, USt3, USt4, USt5, USt6, USt7, USt8, USt9, USt10, USt11, USt12};
int echoes[] = {USe1, USe2, USe3, USe4, USe5, USe6, USe7, USe8, USe9, USe10, USe11, USe12};
long duration, inches, cm;
long duration2, duration3, duration4;

//Analog Light Sensors
const int Li1 = 10;
const int Li2 = 7;
const int Li3 = 8;
const int Li4 = 9;
//Analog Infrared Sensors
const int ir1 = 13;
const int ir2 = 12;
const int ir3 = 11;

//RBG
const int RGB = 2;
//const int SDA = 14;
//const int SCL = 15;
const int INT = 53;
const int zpin = 9;

//States
bool leftSignal;
bool rightSignal;
bool frontSignal;
bool backSignal;

  void Output() {
    pinMode(RGB, OUTPUT);
    for (int i = 0; i<sensorSize; i++) {
      pinMode(transmitters[i], OUTPUT);
    }
    leftSignal = false;                   //check for US sensors 2, 3, 4, 5
    rightSignal = false;                  //check for US sensors 8, 9, 10, 11
    frontSignal = false;                  //check for IR sensor 1 & US sensors 6, 7
    backSignal = false;                   //check for IR sensor 2 & US sensors 11, 12
  }
  void Input () {
    for (int i = 0; i<sensorSize; i++) {
      pinMode(echoes[i], INPUT);
    }
  }
  void ping() {
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], LOW);
    }
    delayMicroseconds(2);
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], HIGH);
    }
    delayMicroseconds(2);
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], LOW);
    }
  }


int retrieve2() {
    int duration = pulseIn(echoes[2], HIGH);
    duration = duration / 74 / 2;
    return duration;
  }
int retrieve3() {
    int duration = pulseIn(echoes[3], HIGH);
    duration = duration / 74 / 2;
    return duration;
  }

  void flagger() {
      if ((retrieve2() < 5) || (retrieve3() <5)) {leftSignal = true;}
      leftSignal = false;
  }

bool obstacleCollision() {
  return false;
}

  void obstacleNearby() {
//    Serial.print(retrieve2());
//    Serial.print("  ");
//    Serial.print(retrieve3());
//    Serial.print("  ");
//    flagger();
    if (!PS3.getButtonPress(R1) && (leftSignal || rightSignal || frontSignal || backSignal)) {
      maxp = wane;
    if (obstacleCollision()) {
      maxp = 0;
      }
    }
    else maxp = tops;
  }

