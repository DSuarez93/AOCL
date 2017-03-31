#include <Sabertooth.h>
#include <QTRSensors.h>
#include <SoftwareSerial.h>

#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2

/*
Quick and easy line following robot using an IR reflectance
array.Connect Vcc and Gnd to the Pololu QTR-8A sensor from 
the arduino. Connect the pins 1,2,3,... to arduino's analog
pins 0,1,2,3,4,5.
If the average of the 3 values of the sensors on the left is
greater than the average of those on the right, then the robot
moves left and vice versa. 

//IMPORTANT CALIBRATION

// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the LED built in to the Arduino on pin 13 while calibration is going on.
// During this phase, you should expose each reflectance sensor to the lightest and 
// darkest readings they will encounter.
// Restart the Arduino, LED will turn on. Cover the sensors for about two seconds, then expose to the white floor or light for two seconds.
// Repeat until Arduino LED turns off. Calibration is complete.


// *** If you want to skip the calibration phase, you can get the raw sensor readings
// (analog voltage readings from 0 to 1023) by calling qtra.read(sensorValues) instead of
// qtra.readLine(sensorValues).
 */

int mid = 0;

/*
 
* Sabertooth Motor Driver
 
*/

const int Mo1 = 17;			    
//connect first motor driver to pin 17
const int Mo2 = 18;

SoftwareSerial SWSerial(NOT_A_PIN, Mo1);
SoftwareSerial SWSerial2(NOT_A_PIN, Mo2);
Sabertooth ST(128, SWSerial);
Sabertooth ST2(129, SWSerial2);

const int Pow = 40;
const int bac = 167;
const int maxp = 255;       //255 is up at full speed
const int maxn = 0;         //0 is down at full speed
const int Stop = 127;       //127 is stop

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog sensors((unsigned char[]) {0, 1, 2, 3, 4, 5}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

void setup()
{

Serial.begin(115200);
ST.motor(Stop);
ST2.motor(Stop);
pinMode(13, OUTPUT);
digitalWrite(13, LOW);
//Pow = 40;
//bac = 167;
Serial.println("Beginning calibration. Expose sensors to black and white floors.");
digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    sensors.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are complete with calibration
Serial.println("Calibration is complete. Reporting white and black floors respectively:  ");

//Compare these values for white floor
for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

//Compare these values for black floor
for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }

  Serial.println();
  Serial.println();
  delay(1000);

}

int change(int variable, int direct) {
  if (direct == 0) {
    if (variable <= maxp) {
      variable+=5;
    }
  }
  else {
    if (variable >= maxn) {
      variable-=5;
    }
  }
  return variable;
}

void loop()
{
digitalWrite(13, HIGH);

unsigned int position = sensors.readLine(sensorValues);
for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.print(position); // comment this line out if you are using raw values  
  Serial.print(" ");
int mx;
int mn;
mx = sensorValues[0];
mn = sensorValues[0];

for(int j=1; j<=5; j++)//Calibrating the sensor, finding max and 
    {                      //min reflectance values.
    if(sensorValues[j] >= mx)
      mx = sensorValues[j];
    if(sensorValues[j] <= mn)
      mn = sensorValues[j];
  }
mid = (mx+mn)/2;

Serial.print(" --- Mid: ");
Serial.println(mid); 

//move forward
/*
 * For ST.motor syntax,
 *    ST or ST2 . motor(1 or 2, integer)
 */

//Pow = change(Pow, 0); // 0 means forward direction
//bac = change(bac, 1); // 1 means backward direction
ST.motor(1, Pow);
ST.motor(2, Pow);
ST2.motor(1, Pow);
ST2.motor(2, Pow);

delay(200);

int comp1;
int comp2;
comp1 = (sensorValues[0]+sensorValues[1]+sensorValues[2]) / 3;
comp2 = ((sensorValues[3]+sensorValues[4]+sensorValues[5]) / 3) + 240;
if(comp1>comp2)//Move right
	{
	ST.motor(1,Pow);
	ST.motor(2,bac);
	ST2.motor(1,Pow);
	ST2.motor(2,bac);
	Serial.println(" RIGHT");
	delay(abs((comp2-comp1)/2));
}
comp2 = comp2 - 480;
if(comp1<comp2)//Move left
	{
	ST.motor(1,bac);
	ST.motor(2,Pow);
	ST2.motor(1,bac);
	ST2.motor(2,Pow);
	Serial.println(" LEFT");
	delay(abs((comp2-comp1)/2));
}

if((sensorValues[0] > mid)&&(sensorValues[5] > mid))//Stop if all the sensors give low 
	{                         //reflectance values
	ST.motor(1, Stop);
	ST.motor(2, Stop);
	ST2.motor(1, Stop);
	ST2.motor(2, Stop);
	Serial.println(" STOP");
//  Pow = 40;
//  bac = 167;
	for(int k=0; k<50; k++)
		{
		digitalWrite(13, HIGH);
		delay(100);
		digitalWrite(13, LOW);
		delay(100);
		}
}
}
