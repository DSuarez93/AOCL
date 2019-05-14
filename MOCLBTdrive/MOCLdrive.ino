//this code shall script the motors to work with the Bluetooth drivers. Use the Sabertooth library.
#include <Sabertooth.h>
#include <SoftwareSerial.h>

void setup() {
  Serial.begin(38400);
  motorSetup();   //MOCLmotors.h
}

void loop() {
  for (int i =0; i<5; i++){
    pow1 = i*10; pow2 = i*10; pow3 = i*10; pow4 = i*10; 
      ST.motor(1, pow1);   
      ST.motor(2, pow2);
      ST2.motor(1, pow3);
      ST2.motor(2, pow4);
      }
}
