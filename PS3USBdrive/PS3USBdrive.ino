/*
  HOW TO OPERATE
  Connect PS3 controller via USB
  HOLD down L1, then use Left Stick to move motors in certain direction
*/
#include <Sabertooth.h>
#include <PS3USB.h>
#include <SoftwareSerial.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

/*
 * Motor Driver
 */
const int Mo1 = 16;
const int Mo2 = 18;
//Sabertooth ST(128);   //first motor drive
SoftwareSerial SWSerial(NOT_A_PIN, Mo1);
Sabertooth ST(128, SWSerial);
Sabertooth ST2(129);         //check second motor drive alternative address
//int power;
int pow1, pow2, pow3, pow4;  //remember to rename pow# with designated motor

/*
 * USB Host Shield
 */
USB Usb;
PS3USB PS3(&Usb); // This will just create the instance

/*
 * Sensors
 */
const int USt1 = 5;
const int USe1 = 6;
long duration, inches, cm;
long duration2, duration3, duration4;

int state = 0;
const int relayState = 8;

  void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  /*
   * Motor Driver
   */
  pinMode(Mo1, OUTPUT);
  pinMode(Mo2, OUTPUT);
  SWSerial.begin(9600);
  SabertoothTXPinSerial.begin(9600);
  //power = 0;
  pow1 = 0; pow2 = 0; pow3 = 0; pow4 = 0;
  Sabertooth::autobaud(SabertoothTXPinSerial);
  state = 0;

  /*
   * USB Host Shield
   */
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
    if (Usb.Init() == -1) {
      Serial.print(F("\r\nOSC did not start"));
      while (1) {
        digitalWrite(13, LOW);
        delay(1200); 
        digitalWrite(13, HIGH);                 //halt
      }
    }
    Serial.println(F("\r\nPS3 USB Library Started"));
    digitalWrite(13, LOW);
    delay(300);
    pinMode(USt1, OUTPUT);
    pinMode(USe1, INPUT);
  }

  int coast(int variable)  {
  //If the value is large, go back to zero quickly.
    if ((variable < -10) || (variable > 10))  {
      if (variable < 0)  {
        variable+=10;
      }
      if (variable > 0)  {
        variable-=10;
      }       
    }
    else  {
  //If close to zero (under abs 10), go back to zero steadily.
      if (variable > 0)  {
        variable --;
      }
      if (variable < 0)  {
        variable++;
      }
    }
    return variable;
  }

  void leftControl()  {
  if (PS3.PS3Connected) {
    if (PS3.getButtonPress(L1))  {
       //Forward
      if (PS3.getAnalogHat(LeftHatY) < 50 )  {      //Adjust for Deadzone
        if (pow1 < 127)  {
          pow1 ++;
        }
        if (pow2 < 127) {
          pow2 ++;
        }
      }
      //Backward
      else if (PS3.getAnalogHat(LeftHatY) > 200)  { //Adjust for Deadzone
          if (pow1 > -127) {
            pow1 --;
          }
          if (pow2 > -127)  {
            pow2 --;
          }
        }
      //Left
      else if (PS3.getAnalogHat(LeftHatX) < 50) {   //Adjust for Deadzone
        if (pow1 < 127)  {
          pow1 ++;
        }
        if (pow2 > -127) {
          pow2 --;
        }
      }
      //Right
      else if (PS3.getAnalogHat(LeftHatX) > 200) {  //Adjust for Deadzone
        if (pow1 > -127)  {
          pow1 --;
        }
        if (pow2 < 127) {
          pow2 ++;
        }
      }      
      else {                //Return to neutral when in Deadzone
        pow1 = coast(pow1);
        pow2 = coast(pow2);
        pow3 = coast(pow3);
        pow4 = coast(pow4);
      }
          
    }
      else {                //Return to neutral when L1 is released
        pow1 = coast(pow1);
        pow2 = coast(pow2);
        pow3 = coast(pow3);
        pow4 = coast(pow4);
      }                     //Disconnect is same as L1 is released.
    }
 }

  void buttonPress()  {
    
   if (PS3.PS3Connected) {
//      if (PS3.getButtonPress(L2))  {
          if (PS3.getButtonClick(CROSS)) {
           if (state == 0) {
            state = 1;
           }
          else {
            state = 0;
          }
//        }
      }
    } 
  }

  void relay()  {         //Affected by ButtonPress
    if (state == 0) digitalWrite(relayState, HIGH);
    else digitalWrite(relayState, LOW);
  }
  
  void ping() {
    digitalWrite(USt1, LOW);
    delayMicroseconds(2);
    digitalWrite(USt1, HIGH);
    delayMicroseconds(10);
    duration = pulseIn(USe1, HIGH);  
    inches = microsecondsToInches(duration);

      if (inches <= 10)   {   //Check if Object is in warning zone
//    Serial.print("Obstacle Close: ");
//    Serial.println("In Front");
//    Serial.println("Behind");
//    Serial.println("Left");
//    Serial.println("Right");

/*        if (power > 50) {
          power--;
        }
        if (power < -50)  {
          power++;
        }
      }

      if (inches <= 3)  {
          if (power > 0)  {
            power-=5;
          }
          if (power < 0)  {
            power+=5;
          }
*/
      }
  }

  void controllerReport() {
  Serial.print("Hats: ");
  Serial.print(PS3.getAnalogHat(LeftHatY));
  Serial.print("    ");
  Serial.println(PS3.getAnalogHat(LeftHatX));
  Serial.print("Power Levels: ");
  Serial.print(pow1);
  Serial.print("    ");
  Serial.print(pow2);
  Serial.print("    ");
  Serial.print(pow3);
  Serial.print("    ");
  Serial.print(pow4);
  Serial.print("    ");
  }
  
long microsecondsToInches(long microseconds)  {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return (microseconds / 74 / 2);
}

void loop() {
  Usb.Task();           //Check PS3 Connection
  leftControl();        //Check for Left Stick
  buttonPress();        //Check for Relay Switch
  relay();              //Update Relay
//  ping();             //Ping Sensors
//  controllerReport(); //Serial Print Controller to Monitor

//Serial.println(power);  //If all motors share "power", they all move in the same direction and speed
//Serial.println(state);
  /*
   * Send final motor values to drivers
   */
  ST.motor(1, pow1);   
  ST.motor(2, pow2);
  ST2.motor(1, pow3);
  ST2.motor(2, pow4);
  delay(50);
  if (pow1 == 0 && pow2 == 0 && pow3 == 0 && pow4 == 0) {
    digitalWrite(13, HIGH);
  }
  else digitalWrite(13, LOW);
