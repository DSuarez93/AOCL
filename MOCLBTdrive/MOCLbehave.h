/*
 * USB Host Shield
 */

#include "MPU6050_Accelerometer.h"
#define wired 0

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
#if (wired == 0)
  BTD Btd(&Usb);      // You have to create the Bluetooth Dongle instance like so
  PS3BT PS3(&Btd);    // This will just create the Bluetooth instance
  //PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57);  //Another way to create instance if address is known
#else
  PS3USB PS3(&Usb); //Use for wired controller, comment PS3BT & Btd(&Usb) declaration
#endif

  //  Deadzone for Controller
const int lowDead = 50;     //lowest is 0
const int highDead = 200;   //highest is 255

  //  Behavioral States
unsigned int standby; //if standby = 1, no movement
bool state;           //software relay
const int relayState = 11;
const int relayState2 = 12;

  //  Connect USB Bluetooth in void setup()
void initConnect() {
  #if !defined(__MIPSEL__)  // - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  while (!Serial); // Wait for serial port to connect
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1) {
      digitalWrite(13, LOW); //halt if USB Host Shield is not connected
      delay(1200);
      digitalWrite(13, HIGH);
      delay(1200);
    }
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  delay(100);
  state = false;             //start in Drive State. true if in Lift State
  standby = 1;               //1 if no input or motionless
  pinMode(relayState, OUTPUT);  pinMode(relayState2, OUTPUT); 
}

void relay()  {             //Affected by PS3.Connected()
    if (standby == 1) {
      pow1 = coast(pow1);
      pow2 = coast(pow2);
      pow3 = coast(pow3);
      pow4 = coast(pow4);
      pow5 = scis;
    }
        //Switch relays
        //Green light means Drive
        //Blue light means Lift
    if (state) { 
      digitalWrite(relayState, HIGH); 
      digitalWrite(relayState2, HIGH);
    }
    else { 
      digitalWrite(relayState, LOW); 
      digitalWrite(relayState2, LOW); 
    }
}

void switchSpeed() {
  if (PS3.getAnalogButton(R2)) {
    if (PS3.getButtonPress(L1)) {
      if (PS3.getButtonPress(CIRCLE)) {
        tops = deft;
      }
      if (PS3.getButtonClick(TRIANGLE)) {
        tops = tops + 10;
      }
    }
  }
}

void buttonPress()  {
      if (PS3.getAnalogButton(L2))  {
          if (PS3.getButtonClick(CROSS)) {
           state = !state;
           if (!state) {
            Serial.print("\r\nDrive State");
            PS3.setLedOff();  //Adjust PS3 LED
            delay(500);
            PS3.setLedOn(LED1);
           }
           else {
            Serial.print("\r\nLift State");
            PS3.setLedOff();  //Adjust PS3 LED
            delay(500);
            PS3.setLedOn(LED4);
           }
        }
      }
 }

   void leftControl()  {
//    if (MPUread() < 20) {              //uncomment for accelerometer
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
                                                                 //spin counterclockwise
              if (pow3 >= -maxp-wane) {
                if (pow3 >= -wane)  {
                        pow3 -= 5;
                      }
                      else pow3 --;
                   }
              if (pow4 >= -maxp-wane) {
                if (pow4 >= -wane)  {
                        pow4 -= 5;
                      }
                      else pow4 --;
                   }
              if (pow2 <= maxp+wane) {
                  if (pow2 <= wane)  {
                        pow2 += 5;
                      }
                      else pow2 ++;
                   }
              if (pow1 <= maxp+wane) {
                  if (pow1 <= wane)  {
                        pow1 += 5;
                      }
                      else pow1 ++;
                   }               
              }
              if ((PS3.getAnalogHat(RightHatX) <= highDead)) {     //right stick is right
                                                                   //spin clockwise
               if (pow3 <= maxp+wane) {
                  if (pow3 <= wane)  {
                        pow3 += 5;
                      }
                      else pow3 ++;
                   }               
               if (pow4 <= maxp+wane) {
                  if (pow4 <= wane)  {
                        pow4 += 5;
                      }
                      else pow4 ++;
                   }
              if (pow2 >= -maxp-wane) {
                  if (pow2 >= -wane)  {
                        pow2 -= 5;
                      }
                      else pow2 --;
                   }                                                     
              if (pow1 >= -maxp-wane) {
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
      if (PS3.getButtonPress(UP)) {   
        pow5 = sciu;                  //raise scissor lift
        if (PS3.getButtonPress(SQUARE)) {
          pow5 +=20;
        }
      }
      else if (PS3.getButtonPress(DOWN)) { 
        pow5 = scid;                  //lower scissor lift
        if (PS3.getButtonPress(SQUARE)) {
          pow5 -=20;
        }        
      }
        //Stop scissor lift motor
      else pow5 = scis;
    } else pow5 = scis;
  } else pow5 = scis;
}

  void controllerReport() {
    
//  Serial.print("Hats: ");
//  Serial.print(PS3.getAnalogHat(LeftHatX));
//  Serial.print("    ");
//  Serial.print(PS3.getAnalogHat(LeftHatY));
//  Serial.print("    ");
//  Serial.print(PS3.getAnalogHat(RightHatY));
  Serial.print("  ::  Power Levels: ");
  Serial.print(pow1);
  Serial.print("  ::  ");
  Serial.print(pow2);
  Serial.print("  ::  ");
  Serial.print(pow3);
  Serial.print("  ::  ");
  Serial.print(pow4);
  Serial.print("  ::  ");
  Serial.print(pow5);
  Serial.print("  ::  ");        
  Serial.println();
  }
/*
    Serial.print(analogRead(3));    
    Serial.print("  ::  ");    
    Serial.print(analogRead(6));
    Serial.print("  ::  ");
    Serial.print(analogRead(2));
    Serial.print("  ::  ");
    Serial.print(analogRead(1));    
//    Serial.print(analogRead(A6));
    /*
    Serial.print("  Outrigger Down Left:   ");
    Serial.print(analogRead(A4));
    Serial.print("  Outrigger Down Right:   ");
    Serial.print(analogRead(A5));
    Serial.print("  Outrigger Up Left:   ");
    Serial.print(analogRead(A1));
    Serial.print("  Outrigger Up Right:   ");
    Serial.print(analogRead(A2));    
    */
