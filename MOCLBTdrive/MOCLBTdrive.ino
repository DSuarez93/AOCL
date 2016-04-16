/*
  HOW TO OPERATE
  Connect PS3 controller via Bluetooth.
  Have Bluetooth adapter inserted into Host Shield, wait 5 seconds, then swap with PS3 USB
  Swap back to Bluetooth dongle and turn on disconnected controller with PS button
  After one setting, the controller should remember the dongle, but reset and repeat above if no connection.
  * HOLD down L1, then use Left Stick to move motors in certain direction
  * Hold down L1, then use Right Stick to turn MOCL
  **  To switch relay, HOLD down L2, then press X
  **  To switch to Scissor, HOLD down L2, then press Triangle
  *   Hold down L2, then use Directional Pad to move Scissor
  *** Turn off controller by holding R2, L1 & pressing PS button
  **  Deactivate obstacle avoidance system by holding down R1
*/

#include <PS3BT.h>
#include <usbhub.h>
#include <Sabertooth.h>
#include <SoftwareSerial.h>
#include "MOCLmotors.h"
#include "MOCLbehave.h"
#include "MOCLsensors.h"
void setup() {
  Serial.begin(38400);
//  Serial.setTimeout(50);
  sensorInit();
  initConnect();
  motorSetup();
}
void loop() {
  Usb.Task();      
  relay();                //Update Relay & Standby

//Only do these methods if PS3 controller is connected.  
  if (PS3.PS3Connected) {
    leftControl();    //Check for Left Stick
    rightControl();   //Check for Right Stick
    scissorLift();    //Check for Directional Pad  
    buttonPress();    //Check for Relay Switch
    if (!PS3.getButtonPress(R1)) {
          if (state) {
      readLight();
      }   else {
      sensorPing();               //Ping Sensors
    }
    }
    
    if (PS3.getButtonPress(R2)) {
      if (PS3.getButtonPress(L1)) {
          if (PS3.getButtonClick(PS)) {
              Serial.print(F("\r\nPS"));
              PS3.disconnect();
          } 
      }
    }
    if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        PS3.printStatusString();
    }

      if (millis() % 10 == 0) {
        controllerReport();   
      }
  }  else standby = 1;
  boundaryCheck(pow1); 
  boundaryCheck(pow2); 
  boundaryCheck(pow3); 
  boundaryCheck(pow4);
  
  /*
   * Send final motor values to drivers
   */

  ST.motor(1, pow1);   
  ST.motor(2, pow2);
  ST2.motor(1, pow3);
  ST2.motor(2, pow4);
  ST3.port().write(pow5);
  
//  delay(10);
  if (pow1 == 0 && pow2 == 0 && pow3 == 0 && pow4 == 0) {
    digitalWrite(13, HIGH);
  }
  else { digitalWrite(13, LOW);  }
}

      /*
      if (PS3.getButtonClick(RIGHT)) {
        //Serial.print(F("\r\nRight"));
          PS3.setLedOff();
          PS3.setLedOn(LED1);
      */
