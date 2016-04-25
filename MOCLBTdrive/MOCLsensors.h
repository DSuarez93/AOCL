/*
 * Sensors
 */
 #include <NewPing.h>
 #define sensorSize 14           //Ultrasonic Sensors
 #define warningDistance 14       //distance (in inches) before slowing down
 #define stoppingDistance 6       //distance (in inches) to stop
 #define ceilingDistance 6        //distance (in inches) to ceiling
 #define interval 91              //milliseconds
 #define switchQuantity 4         //limit switches
 #define lightQuantity 2          //light sensors
 #define lightReference 30
 #define limitReference 30

//Digital Ultrasonic Sensors
const int transmitters[sensorSize] = {48, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 22};
const int echoes[sensorSize] = {49, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 23};
bool flag[sensorSize];            //Obstacle Report
bool allFlags, stopFlag;
long duration, inches;            //duration data
unsigned long timer[sensorSize];  //sensor timer
uint8_t currentSensor;

//Analog Light Sensors
const int light[lightQuantity] = {10, 9};       
long lightSense[lightQuantity];

//Analog Infrared Sensors
/*
 * #define infraredReference 30
 *  #define infraredQuantity 3       //infrared sensors
const int infrared[infraredQuantity] = {13, 12, 11};  
long infSense[infraredQuantity] ;                     
*/

//accelerometer needs a calibration file
//const int SDA = 14;
//const int SCL = 15;
const int INT = 53;   //digital
const int zpin = 9;   //analog

//Limit Switches
/*
 * Bottom of Scissor Lift
 * Top of Scissor Lift
 * Up Left of Outrigger
 * Up right of Outrigger
 */
const int lSwitch[switchQuantity] = { 5, 10, 8, 9};        //digital
const int aSwitch[switchQuantity] = {  3, 6, 2, 1 };        //analog
bool switchAct[switchQuantity];
int switchRead[switchQuantity];
 
/*
 * See playground.arduino.cc/Code/NewPing
 */
 NewPing US[sensorSize] = { 
    NewPing(transmitters[0], echoes[0]),
    NewPing(transmitters[1], echoes[1]),
    NewPing(transmitters[2], echoes[2]),
    NewPing(transmitters[3], echoes[3]),
    NewPing(transmitters[4], echoes[4]),
    NewPing(transmitters[5], echoes[5]),
    NewPing(transmitters[6], echoes[6]),
    NewPing(transmitters[7], echoes[7]),
    NewPing(transmitters[8], echoes[8]),
    NewPing(transmitters[9], echoes[9]),
    NewPing(transmitters[10], echoes[10]),
    NewPing(transmitters[11], echoes[11]),
    NewPing(transmitters[12], echoes[12]),
    NewPing(transmitters[13], echoes[13])
  };

 NewPing ceiling(50, 51);       //check that scissor lift doesn't hit anything above it

  void sensorInit () {  
    timer[0] = millis() + 75;
    for (int i = 0; i<sensorSize; i++) {
      pinMode(transmitters[i], OUTPUT);
      pinMode(echoes[i], INPUT);
      flag[i] = false;
      allFlags = false;
      timer[i+1] = timer[i] + interval;
    }
    Serial.println("US sensors established.");
    duration = 0;    
    for (uint8_t i = 0; i < switchQuantity; i++)    {  pinMode(lSwitch[i], OUTPUT); digitalWrite(lSwitch[i], LOW); }
    Serial.println("Limit Switches established.");
    for (uint8_t i = 0; i < lightQuantity; i++)     {  pinMode(light[i], INPUT);   }
    Serial.println("Beacon Sensors established.");
  }

/*
 * For lifting ordnance
 */

  void readLight() {
    for (uint8_t i = 0; i < lightQuantity; i++) {
      lightSense[i] = analogRead(light[i]);
    }
    if (lightSense[0] > lightReference &&
        lightSense[1] > lightReference) {
      //digitalWrite(RGB[0], LOW);
    } //else digitalWrite(RGB[0], HIGH);
  }

  void ceilingSensor() {
    inches = ceiling.ping()/US_ROUNDTRIP_IN;
    if (inches < ceilingDistance && inches > 0) {
      pow5 = scis;
    }
  }


  /*
   * For driving robot, calls obstacleNearby() & echoCheck()
   */
  
  void obstacleNearby() {
      if (duration > 1 && duration < warningDistance) {
         flag[currentSensor] = true;
         allFlags = true;
         maxp = wane; Serial.print("\nClose  ");
         if (duration < stoppingDistance) {
            stopFlag = true;
            maxp = 0; Serial.print("\nStop  ");
         }
         return;  
      }
      flag[currentSensor] = false;
  }

  void safeReset() {
      for(uint8_t i = 0; i < sensorSize; i++) {
        if (flag[i]) {
          return;
        }
      }
      allFlags = false;
      stopFlag = false;
      maxp = tops;
  }
  
  void echoCheck() {
    if(US[currentSensor].check_timer()) {
      duration = US[currentSensor].ping_result/US_ROUNDTRIP_IN;
      Serial.print(currentSensor);
      Serial.print(" :: ");
      Serial.print(duration);
      Serial.println();
    }
  }

  void sensorPing() {
    for (uint8_t i = 0; i<sensorSize ; i++) {
      if (millis() >= timer[i]) {
        timer[i] += interval * sensorSize;
        obstacleNearby();
        if (i==0 && currentSensor == sensorSize-1) {
            safeReset();
        }
        US[currentSensor].timer_stop();
        currentSensor = i;
        //duration = 0;
        US[currentSensor].ping_timer(echoCheck);
      }
    }
  }  
