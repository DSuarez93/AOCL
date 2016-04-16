/*
 * Sensors
 */
 #define sensorSize 12
 #include <NewPing.h>
 #define warningDistance 6
 #define stoppingDistance 4
 #define interval 191 //milliseconds
 #define switchQuantity 6
 #define lightQuantity 4
 #define infraredQuantity 3
 #define infraredReference 30
 #define lightReference 30
 #define limitReference 30

//Digital Ultrasonic Sensors
const int transmitters[sensorSize] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44};
const int echoes[sensorSize] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45};
long duration, inches;
unsigned long timer[sensorSize];
uint8_t currentSensor;

//Analog Light Sensors
const int light[lightQuantity] = {10, 7, 8, 9};       
long lightSense[lightQuantity];

//Analog Infrared Sensors
const int infrared[infraredQuantity] = {13, 12, 11};  
long infSense[infraredQuantity] ;                     

//RBG
#define RGB 2

//accelerometer needs a calibration file
//const int SDA = 14;
//const int SCL = 15;
const int INT = 53;   //digital
const int zpin = 9;   //analog

//Obstacle States
bool closeFlag;
bool stopFlag;

//Limit Switches
const int lSwitch[switchQuantity] = { 46, 47, 48, 49, 50, 51 };   //digital
const int aSwitch[switchQuantity] = {  3, 4, 5, 6, 1, 2 };        //analog
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
    NewPing(transmitters[11], echoes[11])
  };

  void sensorInit () {
    pinMode(RGB, OUTPUT);
    timer[0] = millis() + 75;
    for (int i = 0; i<sensorSize; i++) {
      pinMode(transmitters[i], OUTPUT);
      pinMode(echoes[i], INPUT);
      timer[i+1] = timer[i] + interval;
    }
    Serial.println("US sensors established.");
    duration = 0;    
    closeFlag = false;
    stopFlag = false;
    for (uint8_t i = 0; i < switchQuantity; i++)    {  pinMode(lSwitch[i], OUTPUT); switchAct[i] = false; }
    Serial.println("Limit Switches established.");
    for (uint8_t i = 0; i < infraredQuantity; i++)  {  pinMode(infrared[i], INPUT); }
    Serial.println("Infrared Sensors established.");
    for (uint8_t i = 0; i < lightQuantity; i++)     {  pinMode(light[i], INPUT);   }
    Serial.println("Beacon Sensors established.");
  }

/*
 * For lifting ordnance
 */

  void readLight() {
    if (state) {
    for (uint8_t i = 0; i < lightQuantity; i++) {
      lightSense[i] = analogRead(light[i]);
    }
    if (lightSense[0] > lightReference &&
        lightSense[1] > lightReference &&
        lightSense[2] > lightReference &&
        lightSense[3] > lightReference) {
      digitalWrite(RGB, LOW);
    } else digitalWrite(RGB, HIGH);
    }
  }

  void readSwitches() {
    
  }
  
  void obstacleNearby() {
      if (duration > 0 && duration < warningDistance) {
         maxp = wane; Serial.print("\nClose  ");
         if (duration < stoppingDistance) {
            maxp = 0; Serial.print("\nStop  ");
         }  
         return;
      }     
      maxp = tops;
  }

  void infCheck() {
    if (false) {
      closeFlag = true;
    }
  }
  
  void echoCheck() {
    if(US[currentSensor].check_timer()) {
      duration = US[currentSensor].ping_result/US_ROUNDTRIP_IN; 
    }
  }

  /*
   * For driving robot, calls obstacleNearby() & echoCheck()
   */
  void sensorPing() {
    if (!state) {
    for (uint8_t i = 0; i<sensorSize ; i++) {
      if (millis() >= timer[i]) {
        timer[i] += interval * sensorSize;
        if (i==0 && currentSensor == sensorSize-1) {
          obstacleNearby();
        }
        US[currentSensor].timer_stop();
        currentSensor = i;
        //duration = 0;
        US[currentSensor].ping_timer(echoCheck);
      }
    }
    }
  }
  
