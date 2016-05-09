/*
 * Sensors
 */
 #include <NewPing.h>
 #define sensorSize 5           //Ultrasonic Sensors
 #define warningDistance 16       //distance (in inches) before slowing down
 #define stoppingDistance 6       //distance (in inches) to stop
 #define maxDistance 40
 #define ceilingDistance 8        //distance (in inches) to ceiling
 #define interval 10            //milliseconds
 #define switchQuantity 4         //limit switches
 #define lightQuantity 2          //light sensors
 #define lightReference 820
 #define limitReference 300

//Digital Ultrasonic Sensors
const int transmitters[sensorSize] =  {26, 32, 36, 42, 22};   //Sensors 4, 7, 11, 14
const int echoes[sensorSize] =        {27, 33, 37, 43, 23};
bool flag[sensorSize];            //Obstacle Report
bool allFlags, stopFlag;
long duration, inches;            //duration data
unsigned long timer[sensorSize];  //sensor timer
uint8_t currentSensor;

//Analog Light Sensors
const int light[lightQuantity] = {13, 12};       
long lightSense[lightQuantity];

//Analog Infrared Sensors

//Accelerometer needs a calibration file
const int INT = 53;   //digital

//Limit Switches
/*
 * Bottom of Scissor Lift
 * Top of Scissor Lift
 * Up Left of Outrigger
 * Up right of Outrigger
 */
const int lSwitch[switchQuantity] = { 5, 10, 8, 9};        //digital
const int iSwitch[switchQuantity] = {8, 9};
const int oSwitch[switchQuantity] = {13, 16};
const int aSwitch[switchQuantity] = {  3, 6, 2, 1 };        //analog
bool switchAct[switchQuantity];
int switchRead[switchQuantity];
 
/*
 * See playground.arduino.cc/Code/NewPing
 */
NewPing US[sensorSize] = {
  NewPing(transmitters[0], echoes[0], maxDistance),
  NewPing(transmitters[1], echoes[1], maxDistance),
  NewPing(transmitters[2], echoes[2], maxDistance),
  NewPing(transmitters[3], echoes[3], maxDistance),
  NewPing(transmitters[4], echoes[4], maxDistance) 
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
    currentSensor = 0;
    duration = 0;    
    for (uint8_t i = 0; i < switchQuantity; i++)    {  pinMode(lSwitch[i], OUTPUT); digitalWrite(lSwitch[i], HIGH); }
    pinMode(oSwitch[0], OUTPUT); pinMode(oSwitch[1], OUTPUT); digitalWrite(oSwitch[0], HIGH); digitalWrite(oSwitch[1], HIGH);
    pinMode(iSwitch[0], INPUT); pinMode(iSwitch[1], INPUT);
    Serial.println("Limit Switches established.");
    for (uint8_t i = 0; i < lightQuantity; i++)     {  pinMode(light[i], INPUT);   }
    Serial.println("Beacon Sensors established.");
  }

/*
 * For lifting ordnance
 */

  char readLight() {
//    Serial.print("    Light Sensors:  ");
    for (uint8_t i = 0; i < lightQuantity; i++) {
      lightSense[i] = analogRead(light[i]);
//      Serial.print(lightSense[i]);
//      Serial.print(" :: ");
    }
    if (lightSense[0] > lightReference &&
        lightSense[1] > lightReference) {
//      Serial.println("Underneath");
      return 'y';     //Yellow if under target
    }
//  Serial.println();
    return 'p';       //Purple if in alignment mode
  }

  char ceilingSensor() {
    inches = ceiling.ping()/US_ROUNDTRIP_IN;
    if (inches < ceilingDistance && inches > 1) {
      pow5 = scis;
      return 'r';    //Red if obstacle detected
    }
    return 'b';      //Blue if in lifting mode
  }

  /*
   * For driving robot, call sensorPing, which calls obstacleNearby() & safeReset(), if R1 or Square is not held
   */
  
  void obstacleNearby() {
      if (duration > 1 && duration < warningDistance) {
         flag[currentSensor] = true;
         allFlags = true;
         maxp = wane; //Serial.print("\nClose  ");
         if (duration < stoppingDistance) {
            stopFlag = true;
            maxp = 0; //Serial.print("\nStop  ");
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
  
  void sensorPing() {
    //Update sensor depending if on time
    if (millis() >= timer[currentSensor]) {
      timer[currentSensor] += interval * sensorSize;
      duration = US[currentSensor].ping()/US_ROUNDTRIP_IN;
/*
      Serial.print("[");
      Serial.print(currentSensor+1);
      Serial.print("] = ");
      Serial.print(duration);
      Serial.print(" : : ");
*/
    //Use duration found to determine of obstacle is nearby. Reduce top speed as needed
      obstacleNearby();
    
    //After one complete round, check if anything was detected. If not, reset the top speed
    if(currentSensor >= sensorSize-1) {
    currentSensor = 0;
    safeReset();
//    Serial.println();
    }
    else currentSensor++;
    }
  }  
