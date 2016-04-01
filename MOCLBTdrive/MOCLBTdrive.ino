/*
  HOW TO OPERATE
  Connect PS3 controller via Bluetooth.
  Have Bluetooth adapter inserted into Host Shield, wait 5 seconds, then swap with PS3 USB
  Swap back to Bluetooth dongle and turn on disconnected controller with PS button
  After one setting, the controller should remember the dongle, but reset and repeat above if no connection.
  HOLD down L1, then use Left Stick to move motors in certain direction
  To switch relay, HOLD down L2, then press X
  To switch to Scissor, HOLD down L2, then press Triangle
  Hold down L1, then use Right Stick to move Scissor
*/
/*
      if (PS3.getAnalogButton(CIRCLE)) {
        Serial.print(F("\r\nCircle: "));
        Serial.print(PS3.getAnalogButton(CIRCLE));
      }
*/
#include <PS3BT.h>
#include <usbhub.h>
#include <Sabertooth.h>
#include <SoftwareSerial.h>
#include "MOCLmotors.h"
#include "MOCLbehave.h"
#include "MOCLsensors.h"

void setup() {
  Serial.begin(115200);
  initConnect();
//  motorSetup();
//  Output();
//  Input();
}
void loop() {
  Usb.Task();
//leftControl();        //Check for Left Stick
//rightControl();       //Check for Right Stick
buttonPress();        //Check for Relay Switch
//relay();              //Update Relay & Standby
//ping();               //Ping Sensors
  /*
   * Send final motor values to drivers
   */
//  ST.motor(1, pow1);   
//  ST.motor(2, pow2);
//  ST2.motor(1, pow3);
//  ST2.motor(2, pow4);
  //ST3.drive(pow5);
  //ST.turn(power);
  if (PS3.PS3Connected) {
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
     if (PS3.getButtonClick(TRIANGLE))
        Serial.print(F("\r\nTraingle"));
      if (PS3.getButtonClick(CROSS))
        Serial.print(F("\r\nCross"));
      if (PS3.getButtonClick(SQUARE))
        Serial.print(F("\r\nSquare"));
      /*
      if (PS3.getButtonClick(UP)) {
        //Serial.print(F("\r\nUp"));
          PS3.setLedOff();
          PS3.setLedOn(LED4);
      }
      if (PS3.getButtonClick(RIGHT)) {
        //Serial.print(F("\r\nRight"));
          PS3.setLedOff();
          PS3.setLedOn(LED1);
      }
      if (PS3.getButtonClick(DOWN)) {
        //Serial.print(F("\r\nDown"));
          PS3.setLedOff();
          PS3.setLedOn(LED2);
      }
      if (PS3.getButtonClick(LEFT)) {
        //Serial.print(F("\r\nLeft"));
          PS3.setLedOff();
          PS3.setLedOn(LED3);
      }
      */
  }
  
  delayMicroseconds(10);
  if (pow1 == 0 && pow2 == 0 && pow3 == 0 && pow4 == 0) {
    digitalWrite(13, HIGH);
  }
  else digitalWrite(13, LOW);
  /*
  if (pow1 > 0 && pow2 > 0 && pow3 > 0 && pow4 > 0) {
      PS3.setLedOff();
      PS3.setLedOn(LED1);
      PS3.setLedOn(LED2); 
  }
  */
}
