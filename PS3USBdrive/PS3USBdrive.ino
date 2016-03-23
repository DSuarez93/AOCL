/*
  HOW TO OPERATE
  Connect PS3 controller via USB
  HOLD down L1, then use Left Stick to move motors in certain direction
  To switch relay, HOLD down L2, then press X
  To switch to Scissor, HOLD down L2, then press Triangle
  Hold down L1, then use Right Stick to move Scissor
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
const int Mo1 = 17;
const int Mo2 = 18;
const int Mo3 = 14;
//Sabertooth ST(128);                       //first motor drive
SoftwareSerial SWSerial(NOT_A_PIN, Mo1);
SoftwareSerial SWSerial2(NOT_A_PIN, Mo2);
//SoftwareSerial SWSerial3(NOT_A_PIN, Mo3);
Sabertooth ST(128, SWSerial);
Sabertooth ST2(129, SWSerial2);
//Sabertooth ST2(129);                        //check second motor drive alternative address
//Sabertooth ST3(128, SWSerial3);               //Does third motor need a different address?
//int power;
int pow1, pow2, pow3, pow4, pow5;  //remember to rename pow# with designated motor
const int maxp = 40;        //127 is the fastest a motor can go
const int wane = 10;         //Transition speed
const int scis = 127;        //For Scissor Lift Drive, 127 is stop
const int sciu = 200;        //255 is up at full speed
const int scid = 55;         //0 is down at full speed
/*
 * USB Host Shield
 */
USB Usb;
PS3USB PS3(&Usb); // This will just create the instance
const int lowDead = 50;
const int highDead = 200;

/*
 * Sensors
 */
//Digital Ultrasonic Sensors
//t means transmit, e means echo
const int USt1 = 22;                        //  Mo1.1         Mo2.1
const int USe1 = 23;                        //  (-)           (+)     Forward  
const int USt2 = 24;                        //  (+)           (-)     Backward
const int USe2 = 25;                        //
const int USt3 = 26;                        //
const int USe3 = 27;                        //
const int USt4 = 28;                        //
const int USe4 = 29;                        //
const int USt5 = 30;                        //  Mo1.2         Mo2.2
const int USe5 = 31;                        //  (-)           (+)     Forward
const int USt6 = 32;                        //  (+)           (-)     Backward
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

/*
 *  Behavioral States
 */
 bool state;
 bool Oswitch;
int standby;
int detectF;
int detectB;
int detectL;
int detectR;

const int relayState = 6;
const int relayState2 = 7;

    void Output() {
    pinMode(RGB, OUTPUT);
    //Front Transmitters
    pinMode(USt1, OUTPUT);
    pinMode(USt2, OUTPUT);
    pinMode(USt3, OUTPUT);
    pinMode(USt4, OUTPUT);
    //Rear Transmitters
    pinMode(USt5, OUTPUT);
    pinMode(USt6, OUTPUT);
    pinMode(USt7, OUTPUT);
    pinMode(USt8, OUTPUT);
    //Side Transmitters
    pinMode(USt9, OUTPUT);
    pinMode(USt10, OUTPUT);
    pinMode(USt11, OUTPUT);
    pinMode(USt12, OUTPUT);
  }
  void Input () {
      //Front Echoes
    pinMode(USe1, INPUT);
    pinMode(USe2, INPUT);
    pinMode(USe3, INPUT);
    pinMode(USe4, INPUT);
      //Rear Echoes
    pinMode(USe5, INPUT);
    pinMode(USe6, INPUT);
    pinMode(USe7, INPUT);
    pinMode(USe8, INPUT);
      //Side Echoes
    pinMode(USe9, INPUT);
    pinMode(USe10, INPUT);
    pinMode(USe11, INPUT);
    pinMode(USe12, INPUT);
  }
  
  void setup() {
    //baud depends on communication
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    /*
    * Motor Driver
    */
    pinMode(Mo1, OUTPUT);
    pinMode(Mo2, OUTPUT);
    pinMode(Mo3, OUTPUT);
    SWSerial.begin(9600);                   //Must be 9600
    SWSerial2.begin(9600);                  //Must be 9600
//    SWSerial3.begin(9600);                  //Must be 9600
    SabertoothTXPinSerial.begin(9600);      //Must be 9600
    //power = 0;
    pow1 = 0; pow2 = 0; pow3 = 0; pow4 = 0; pow5 = 127;
    ST.autobaud();
    ST2.autobaud();
//    ST3.autobaud();
    state = true;
    Oswitch = true;
    standby = 1;
    detectF = 0;
    detectB = 0;
    detectL = 0;
    detectR = 0;
    pinMode(relayState, OUTPUT);
    pinMode(relayState2, OUTPUT);
    digitalWrite(relayState, LOW);
    digitalWrite(relayState2, LOW);

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
    Output();
    Input();
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

  void rightControl() {
    if (PS3.PS3Connected) {
      if (PS3.getButtonPress(L1))  {
//        DeadZone
        if((PS3.getAnalogHat(RightHatY) < lowDead) || 
           (PS3.getAnalogHat(RightHatY) > highDead)) {
              standby = 0;
              if (PS3.getAnalogHat(RightHatY) >= highDead) {
                pow5 = sciu;
              }
              if (PS3.getAnalogHat(RightHatY) <= lowDead) {
                pow5 = scid;
              }
           }
      }
      else {
        standby = 1;
      }
    }
    else {
      standby = 1;
    }
  }

  void leftControl()  {
  if (PS3.PS3Connected) {
    if (PS3.getButtonPress(L1))  {
        
//        DeadZone
        if((PS3.getAnalogHat(LeftHatX) < lowDead) || 
           (PS3.getAnalogHat(LeftHatX) > highDead) ||
           (PS3.getAnalogHat(LeftHatY) < lowDead) ||
           (PS3.getAnalogHat(LeftHatY) > highDead)) {
           standby = 0;
//         Mo1.1
             if((PS3.getAnalogHat(LeftHatX) >= 0) && 
                 (PS3.getAnalogHat(LeftHatX) <= highDead) && 
                 (PS3.getAnalogHat(LeftHatY) >= 0) &&
                 (PS3.getAnalogHat(LeftHatY) <= highDead))  {
                  if (pow3 <= maxp) {
                      if (pow3 <= wane)  {
                        pow3 += 5;
                      }
                      else pow3 ++;
                   }
             }
             else {
               if (pow3 >= -maxp)  {   
                  if (pow3 >= -wane)  {
                        pow3 -= 5;
                  }
                  else pow3 --;
               }
             }    
//         Mo1.2
            if((PS3.getAnalogHat(LeftHatX) >= 0 ) &&
               (PS3.getAnalogHat(LeftHatX) <= highDead) &&
               (PS3.getAnalogHat(LeftHatY) >= lowDead) &&
               (PS3.getAnalogHat(LeftHatY) <= 255)) {
                  if (pow4 >= -maxp) {
                      if (pow4 >= -wane)  {
                        pow4 -= 5;
                      }
                      else pow4 --;
                   }
             }
             else {
               if (pow4 <= maxp)  {   
                  if (pow4 <= wane)  {
                        pow4 += 5;
                  }
                  else pow4 ++;
               }
             }   
//          Mo2.1
             if((PS3.getAnalogHat(LeftHatX) >= lowDead) && 
                 (PS3.getAnalogHat(LeftHatX) <= 255) && 
                 (PS3.getAnalogHat(LeftHatY) >= 0) &&
                 (PS3.getAnalogHat(LeftHatY) <= highDead))  {
                   if (pow1 <= maxp) {
                      if (pow1 <= wane)  {
                        pow1 += 5;
                      }
                      else pow1 ++;
                   }
             }
             else {
               if (pow1 >= -maxp)  {   
                  if (pow1 >= -wane)  {
                        pow1 -= 5;
                  }
                  else pow1 --;
               }
             }    
//          Mo2.2 
             if((PS3.getAnalogHat(LeftHatX) >= lowDead) && 
                 (PS3.getAnalogHat(LeftHatX) <= 255) && 
                 (PS3.getAnalogHat(LeftHatY) >= lowDead) &&
                 (PS3.getAnalogHat(LeftHatY) <= 255))  {
                   if (pow2 >= -maxp) {
                      if (pow2 >= -wane)  {
                        pow2 -= 5;
                      }
                      else pow2 --;
                   }
             }
             else {
               if (pow2 <= maxp)  {   
                  if (pow2 <= wane)  {
                        pow2 += 5;
                  }
                  else pow2 ++;
               }
             }       
        }
        else {
            standby = 1;
        }
    }
      else {                //Return to neutral when L1 is released
        standby = 1;
      }                     //Disconnect is same as L1 is released.
    }
 }

  void boundaryCheck(int variable)  {
    if (variable > maxp) {
      variable = maxp;
    }
    if (variable < -maxp) {
      variable = -maxp;
    }
  }
  
  void buttonPress()  {
    
   if (PS3.PS3Connected) {
      if (PS3.getButtonPress(L2))  {
          if (PS3.getButtonClick(CROSS)) {
           if (!state) {
            state = true;
           }
          else {
            state = false;
          }
        }
          if (PS3.getButtonClick(TRIANGLE)) {
            if (!Oswitch) {
              Oswitch = true;
            }
            else {
              Oswitch = false;
            }
          }
      }
    } 
  }

  void relay()  {         //Affected by ButtonPress
    if (standby == 1) {
      pow1 = coast(pow1);
      pow2 = coast(pow2);
      pow3 = coast(pow3);
      pow4 = coast(pow4);
      pow5 = scis;
    }
    if (!state) { 
      digitalWrite(relayState, HIGH); 
      digitalWrite(relayState2, HIGH); 
    }
    else { 
      digitalWrite(relayState, LOW); 
      digitalWrite(relayState2, LOW); 
    }
  }
  
  void ping() {
    digitalWrite(USt1, LOW);
    digitalWrite(USt2, LOW);  
    digitalWrite(USt3, LOW);  
    digitalWrite(USt4, LOW); 
    digitalWrite(USt5, LOW);  
    digitalWrite(USt6, LOW);  
    digitalWrite(USt7, LOW);  
    digitalWrite(USt8, LOW);    
    digitalWrite(USt9, LOW);  
    digitalWrite(USt10, LOW);  
    digitalWrite(USt11, LOW);  
    digitalWrite(USt12, LOW);     
    delayMicroseconds(2);
    digitalWrite(USt1, HIGH);
    digitalWrite(USt2, HIGH);  
    digitalWrite(USt3, HIGH);  
    digitalWrite(USt4, HIGH); 
    digitalWrite(USt5, HIGH);  
    digitalWrite(USt6, HIGH);  
    digitalWrite(USt7, HIGH);  
    digitalWrite(USt8, HIGH);    
    digitalWrite(USt9, HIGH);  
    digitalWrite(USt10, HIGH);  
    digitalWrite(USt11, HIGH);  
    digitalWrite(USt12, HIGH);     
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
  Serial.print(PS3.getAnalogHat(LeftHatX));
  Serial.print("    ");
  Serial.print(PS3.getAnalogHat(LeftHatY));
  Serial.print("    ");
  Serial.println(PS3.getAnalogHat(RightHatY));
  Serial.print("Power Levels: ");
  Serial.print(pow1);
  Serial.print("    ");
  Serial.print(pow2);
  Serial.print("    ");
  Serial.print(pow3);
  Serial.print("    ");
  Serial.print(pow4);
  Serial.print("    ");
  Serial.print(pow5);
  Serial.print("    ");
  Serial.print("States: ");
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
  rightControl();       //Check for Right Stick
  buttonPress();        //Check for Relay Switch
  relay();              //Update Relay & Standby
//  ping();             //Ping Sensors
  controllerReport(); //Serial Print Controller to Monitor

Serial.print("Relay pins:   "); Serial.print(digitalRead(relayState)); Serial.print("     "); Serial.print(digitalRead(relayState2)); Serial.print("     ");
  /*
   * Send final motor values to drivers
   */
  ST.motor(1, pow1);   
  ST.motor(2, pow2);
  ST2.motor(1, pow3);
  ST2.motor(2, pow4);
  //ST3.drive(pow5);
  //ST.turn(power);
  delay(50);
  if (pow1 == 0 && pow2 == 0 && pow3 == 0 && pow4 == 0) {
    digitalWrite(13, HIGH);
  }
  else digitalWrite(13, LOW);
}

