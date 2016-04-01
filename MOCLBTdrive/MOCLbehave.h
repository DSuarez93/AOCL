/*
 * USB Host Shield
 */

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
const int lowDead = 50;
const int highDead = 200;
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
  state = true;
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

void buttonPress()  {
    
   if (PS3.PS3Connected) {
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
