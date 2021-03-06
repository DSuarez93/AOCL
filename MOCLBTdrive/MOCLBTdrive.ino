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
#include "MPU6050_Start.h"
#define RGBQuantity 3            //LED lights
const int RGB[RGBQuantity] = {
                              2,  //Green [0]
                              3,  //Red   [1]
                              4}; //Blue  [2]
char color;

  void RGBoutput(char color) {
    if (color == 'r') {         //Red
    digitalWrite(RGB[0], LOW);
    digitalWrite(RGB[1], HIGH);
    digitalWrite(RGB[2], HIGH);
      
    }
    if (color == 'g') {         //Green
      digitalWrite(RGB[0], HIGH);
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], HIGH);
      
    }
    if (color == 'y') {         //Yellow
      digitalWrite(RGB[0], LOW);
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], HIGH);
     
    }
    if (color == 'b') {         //Blue
      digitalWrite(RGB[0], HIGH);
      digitalWrite(RGB[1], HIGH);
      digitalWrite(RGB[2], LOW);
      
    }
    if (color == 'w') {         //White
      digitalWrite(RGB[0], LOW);      
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], LOW);            
      
    }
    if (color == 'p') {         //Purple
      digitalWrite(RGB[0], HIGH);      
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], LOW);
      
    }
    if (color == 't') {         //Cyan
      digitalWrite(RGB[0], LOW);      
      digitalWrite(RGB[1], HIGH);
      digitalWrite(RGB[2], LOW);
    }
    if (color == 'o') {         //Off
      digitalWrite(RGB[0], LOW);      
      digitalWrite(RGB[1], LOW);
      digitalWrite(RGB[2], LOW);
    }
  }

void setup() {
  Serial.begin(38400);
  pinMode(RGB[0], OUTPUT);   digitalWrite(RGB[0], HIGH);
  pinMode(RGB[1], OUTPUT);   digitalWrite(RGB[1], HIGH);
  pinMode(RGB[2], OUTPUT);   digitalWrite(RGB[2], HIGH);
  color = 'w';
  sensorInit();   //MOCLsensors.h
  initConnect();  //MOCLbehave.h
  motorSetup();   //MOCLmotors.h
  MPUinit();      //MPU6050_Accelerometer.h
}
void loop() {
  Usb.Task();      
  relay();                //Update Relay & Standby
  switchSpeed();
  
//Only do these methods if PS3 controller is connected.  
  if (PS3.PS3Connected) {
  
  //Methods to disconnect controller or output status to serial monitor.
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
    
  //Change inputs according to controller
    leftControl();    //Check for Left Stick
    rightControl();   //Check for Right Stick
    scissorLift();    //Check for Directional Pad  
    buttonPress();    //Check for Relay Switch
  //Then adjust paramters according to sensors
/*
//    if ((digitalRead(iSwitch[0]) == 0) || (digitalRead(iSwitch[1]) == 0)) {
//      pow5 = scis; }
*/    
    if (!PS3.getButtonPress(R1)) {
          if (state) {                                    //Blue = Lifting State
            color = 'b';
            //color = ceilingSensor();                    //Red stop, Obstacle above
          }   else {                                      //Green = Driving State
                if (PS3.getButtonPress(SQUARE)) {         //Purple = Enter Alignment Mode
                 color = readLight();                     //Yellow means alignmnet is good
                 maxp = wane*2+4;
                } else {
                      color = 'g';
                      sensorPing();                       //Ping Sensors
                      if (allFlags) {
                          if (millis() % 1000 <= 500) {
                           color = 'r';                   //Flashing, Obstacle nearby
                          }
                          else {color = 'o';}
                          if (stopFlag) {                 //Solid red stop, Obstacle too close
                              color = 'r';
                          }
                      }
                    }
               }
      } else {    //Deactivate sensors when R1 is held
         maxp = tops;
      }

    //Debug print report of controller or sensors to serial monitor
      if (millis() % 100 == 0) {
        controllerReport();   
        //Serial.print(digitalRead(iSwitch[0]));
        //Serial.println(digitalRead(iSwitch[1]));
      }
  }  else {
    standby = 1;                     //Standby when PS3 controller is disconnected
    color = 'w';
  }
      //Update all power variables so they don't exceed impossible values
  pow1 = boundaryCheck(pow1);
  pow2 = boundaryCheck(pow2); 
  pow3 = boundaryCheck(pow3); 
  pow4 = boundaryCheck(pow4);
      
      //Update color LED 
  RGBoutput(color);
  
  
     //Send final motor values to drivers

    if (maxp == 0) {
      pow1 = 0;
      pow2 = 0;
      pow3 = 0;
      pow4 = 0;
      ST.stop();
      ST2.stop();
    } else {
      ST.motor(1, pow1);   
      ST.motor(2, pow2);
      ST2.motor(1, pow3);
      ST2.motor(2, pow4);
    }
    ST3.port().write(pow5);
  
//  Light Debug LED if motors are at zero
  if (pow1 == 0 && pow2 == 0 && pow3 == 0 && pow4 == 0) {
    digitalWrite(13, HIGH);
  }
  else { digitalWrite(13, LOW);  }
}
