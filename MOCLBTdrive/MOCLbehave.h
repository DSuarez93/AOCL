/*
 * USB Host Shield
 */

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
const int lowDead = 50;
const int highDead = 200;
/*
 *  Behavioral States
 */
unsigned int standby; //standby = 1, no movement
bool state;           //software relay
const int relayState = 10;
const int relayState2 = 11;

void initConnect() {
  #if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1) {
      digitalWrite(13, LOW); //halt
      delay(1200);
      digitalWrite(13, HIGH);
      delay(1200);
    }
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  delay(100);
  state = false;        //state in Drive State. true if in Lift State
  standby = 1;
  pinMode(relayState, OUTPUT);
  pinMode(relayState2, OUTPUT); 
}

void relay()  {         //Affected by ButtonPress
    if (standby == 1) {
      pow1 = coast(pow1);
      pow2 = coast(pow2);
      pow3 = coast(pow3);
      pow4 = coast(pow4);
      pow5 = scis;
    }
    if (state) { 
      digitalWrite(relayState, HIGH); 
      digitalWrite(relayState2, HIGH); 
    }
    else { 
      digitalWrite(relayState, LOW); 
      digitalWrite(relayState2, LOW); 
    }
}

void buttonPress()  {
      if (PS3.getAnalogButton(L2))  {
          if (PS3.getButtonClick(CROSS)) {
           state = !state;
           if (!state) {
            Serial.print("\r\nDrive State");
            PS3.setLedOff();
            delay(500);
            PS3.setLedOn(LED1);
           }
           else {
            Serial.print("\r\nLift State");
            PS3.setLedOff();
            delay(500);
            PS3.setLedOn(LED4);
           }
        }
          if (PS3.getButtonClick(TRIANGLE)) {
          }
      }
 }

   void leftControl()  {
//    if (analogRead(zpin) < 20) {              //uncomment for accelerometer
    if (PS3.getButtonPress(L1))  {
//        DeadZone
        if(!state && (
           (PS3.getAnalogHat(LeftHatX) < lowDead) || 
           (PS3.getAnalogHat(LeftHatX) > highDead) ||
           (PS3.getAnalogHat(LeftHatY) < lowDead) ||
           (PS3.getAnalogHat(LeftHatY) > highDead))) {
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
        } else standby = 1;    //Return when inside DeadZone
    }  else standby = 1;       //Return to neutral when L1 is released                     
//    }                     //Uncomment for accelerometer
 }

void rightControl() {
    if (standby != 0 && !state) {
      if (PS3.getButtonPress(L1))  {
        if ( //        DeadZone
           (PS3.getAnalogHat(RightHatX) < lowDead) ||
           (PS3.getAnalogHat(RightHatX) > highDead)) {
            standby = 2;
              if((PS3.getAnalogHat(RightHatX) >= lowDead)) {     //right stick is left
              if (pow3 >= -maxp) {
                if (pow3 >= -wane)  {
                        pow3 -= 5;
                      }
                      else pow3 --;
                   }
              if (pow4 >= -maxp) {
                if (pow4 >= -wane)  {
                        pow4 -= 5;
                      }
                      else pow4 --;
                   }
              if (pow2 <= maxp) {
                  if (pow2 <= wane)  {
                        pow2 += 5;
                      }
                      else pow2 ++;
                   }
              if (pow1 <= maxp) {
                  if (pow1 <= wane)  {
                        pow1 += 5;
                      }
                      else pow1 ++;
                   }               
              }
              if ((PS3.getAnalogHat(RightHatX) <= highDead)) {     //right stick is right
               if (pow3 <= maxp) {
                  if (pow3 <= wane)  {
                        pow3 += 5;
                      }
                      else pow3 ++;
                   }               
               if (pow4 <= maxp) {
                  if (pow4 <= wane)  {
                        pow4 += 5;
                      }
                      else pow4 ++;
                   }
              if (pow2 >= -maxp) {
                  if (pow2 >= -wane)  {
                        pow2 -= 5;
                      }
                      else pow2 --;
                   }                                                     
              if (pow1 >= -maxp) {
                  if (pow1 >= -wane)  {
                        pow1 -= 5;
                      }
                      else pow1 --;
                   }
              }
           }  
    } 
  } 
}

void scissorLift() {
  if (state) {
    if (PS3.getAnalogButton(L2)) {
      standby = 3;                    //go if outriggers are down
      //if(switchRead[1] && switchRead[2] && switchRead[4] && switchRead[5])
      if (PS3.getButtonPress(UP)) {   //&& switchRead[3] is not hit
        pow5 = scid;                  //raise scissor lift
        if (PS3.getButtonPress(SQUARE)) {
          pow5 +=40;
        }
      }
      else if (PS3.getButtonPress(DOWN)) { //&&switchRead[0] is not hit
        pow5 = sciu;                  //lower scissor lift
      }
      //else pow5 = scis;
      else pow5 = scis;
    } else pow5 = scis;
  } else pow5 = scis;
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
  Serial.print("    ");

  }

/*
      if (PS3.getAnalogButton(CIRCLE)) {
        Serial.print(F("\r\nCircle: "));
        Serial.print(PS3.getAnalogButton(CIRCLE));
      }
*/

