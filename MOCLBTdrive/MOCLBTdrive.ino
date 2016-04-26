/*
  HOW TO OPERATE
  Connect PS3 controller via Bluetooth.
  Have Bluetooth adapter inserted into Host Shield, wait 5 seconds, then swap with PS3 USB
  Swap back to Bluetooth dongle and turn on disconnected controller with PS button
  After one setting, the controller should remember the dongle, but reset and repeat above if no connection.
  * HOLD down L1, then use Left Stick to move motors in certain direction
  * Hold down L1, then use Right Stick to turn MOCL
  *   Hold down L2, then use Directional Pad to move Scissor
  **  To switch relay, HOLD down L2, then press X
  **  To ________, HOLD down L2, then press Triangle

  **  Deactivate obstacle avoidance system by holding down R1
  **  Reduce lift speed by holding Square while pressing Direcitonal Pad
  *** Turn off controller by holding R2, L1 & pressing PS button
*/

#include <PS3BT.h>
#include <usbhub.h>
#include <Sabertooth.h>
#include <SoftwareSerial.h>
#include "MOCLmotors.h"
#include "MOCLbehave.h"
#include "MOCLsensors.h"
 #define RGBQuantity 3            //LED lights
 //RGB
//Green, Red, Blue
const int RGB[RGBQuantity] = {2, 3, 4};
String color;

  void RGBoutput(String color) {
    if (color == "Red") {
      digitalWrite(RGB[0], HIGH);
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], HIGH);
    }
    if (color == "Green") {
      digitalWrite(RGB[0], LOW);
      digitalWrite(RGB[1], HIGH);
      digitalWrite(RGB[2], HIGH);
    }
    if (color == "Yellow") {
      digitalWrite(RGB[0], LOW);
      digitalWrite(RGB[1], HIGH);
      digitalWrite(RGB[2], LOW);
    }
    if (color == "Blue") {
      digitalWrite(RGB[0], HIGH);
      digitalWrite(RGB[1], HIGH);
      digitalWrite(RGB[2], LOW);
    }
    if (color == "White") {
      digitalWrite(RGB[0], LOW);      
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], LOW);            
    }
    if (color == "Purple") {
      digitalWrite(RGB[0], HIGH);      
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], LOW);            
    }    
  }

void setup() {
  Serial.begin(38400);
//  Serial.setTimeout(50);
  pinMode(RGB[0], OUTPUT);
  pinMode(RGB[1], OUTPUT);
  pinMode(RGB[2], OUTPUT);
  digitalWrite(RGB[0], HIGH);
  digitalWrite(RGB[1], HIGH);
  digitalWrite(RGB[2], HIGH);
  color = "White";

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
//    readSwitches();
    if (!PS3.getButtonPress(R1)) {
          if (state) {            //Blue = Lifting State
            if (color != "Blue") {color = "Blue";}
            ceilingSensor();
            color = readLight();
          }   else {              //Purple = Driving State
                if (color != "Purple") {color = "Purple";}
                sensorPing();               //Ping Sensors
               if (allFlags) {
                if (millis() % 1000 <= 500) {color = "Red";}
                else {color = "White";}
                if (stopFlag) {   //Red = Stop, Obstacle nearby
                  {if (color != "Red") color = "Red";}
                }
          }
              }
    } else {
         maxp = tops;
      }
    //Disconnect Controller
    if (PS3.getButtonPress(R2)) {           
      if (PS3.getButtonPress(L1)) {
          if (PS3.getButtonClick(PS)) {
              Serial.print(F("\r\nPS"));
              PS3.disconnect();
          } 
      }
    }
    //Print Controller Status
    if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        PS3.printStatusString();
    }
    //Print report of controller or sensors
      if (millis() % 100 == 0) {
        controllerReport();   
      }
  }  else {
    standby = 1;                     //Standby when PS3 controller is disconnected
    color = "White";
  }
  boundaryCheck(pow1);
  boundaryCheck(pow2); 
  boundaryCheck(pow3); 
  boundaryCheck(pow4);
  RGBoutput(color);
  
  /*
   * Send final motor values to drivers
   */
  ST.motor(1, pow1);   
  ST.motor(2, pow2);
  ST2.motor(1, pow3);
  ST2.motor(2, pow4);
  ST3.port().write(pow5);
  
//  Light Debug LED if motors are at zero
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
