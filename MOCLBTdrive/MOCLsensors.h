/*
 * Sensors
 */
 #define sensorSize 12
 #include <NewPing.h>
 #define warningDistance 8
 #define stoppingDistance 4
//Digital Ultrasonic Sensors
//t means transmit, e means echo
/*
 * See playground.arduino.cc/Code/NewPing
 * NewPing US[sensorSize] = { 
 * NewPing(USt[0], USe[0]),
 NewPing(USt[1], USe[1]),
 NewPing(USt[2], USe[2]),
 NewPing(USt[3], USe[3]),
 NewPing(USt[4], USe[4]),
 NewPing(USt[5], USe[5]),
 NewPing(USt[6], USe[6]),
 NewPing(USt[7], USe[7]),
 NewPing(USt[8], USe[8]),
 NewPing(USt[9], USe[9]),
 NewPing(USt[10], USe[10]),
 NewPing(USt[11], USe[11]),
 NewPing(USt[12], USe[12]),
 * };
 */
const int USt[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44};
const int USe[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45};
const int transmitters[] = {USt[0], USt[1], USt[2], USt[3], USt[4], USt[5], USt[6], USt[7], USt[8], USt[9], USt[10], USt[11]};
const int echoes[] = {USe[0], USe[1], USe[2], USe[3], USe[4], USe[5], USe[6], USe[7], USe[8], USe[9], USe[10], USe[11]};
long duration, inches, cm;

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
bool signals[sensorSize];
//bool leftSignal;
//bool rightSignal;
//bool frontSignal;
//bool backSignal;
bool stopFlag;

  void Output() {
    pinMode(RGB, OUTPUT);
    for (int i = 0; i<sensorSize; i++) {
      pinMode(transmitters[i], OUTPUT);
      signals[i] = false;
    }
    /*
    leftSignal = false;                   //check for US sensors 2, 3, 4, 5
    rightSignal = false;                  //check for US sensors 8, 9, 10, 11
    frontSignal = false;                  //check for IR sensor 1 & US sensors 6, 7
    backSignal = false;                   //check for IR sensor 2 & US sensors 11, 12
    */
  }
  void Input () {
    for (int i = 0; i<sensorSize; i++) {
      pinMode(echoes[i], INPUT);
    }
    duration = 0;
    stopFlag = false;
  }
  void ping() {
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], LOW);
    }
    delayMicroseconds(10);
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], HIGH);
    }
    delayMicroseconds(2);
    for (int i = 0; i<sensorSize; i++) {
      digitalWrite(transmitters[i], LOW);
    }
  }

  void checkIn(int x) {
    duration = pulseIn(echoes[x], HIGH);
    duration = duration /74/2;
      if (duration < warningDistance && duration > 0) {
          signals[x] = true;
          if (duration < stoppingDistance) {
              stopFlag = true;
          } else stopFlag = false;
      } else {
        stopFlag = false;
        signals[x] = false;
      }
  }
  
  void obstacleNearby() {

    if (!PS3.getButtonPress(R1)){
      for (int i = 0; i<sensorSize; i++) {
          checkIn(i);
       }
      if(stopFlag) {
        maxp = 0;
        return;
      }
      for (int i = 0; i<sensorSize; i++) {
        if(signals[i]) {
          maxp = wane;
          return;
        }
      }
       maxp = tops;
    }
    else maxp = tops;
  }
