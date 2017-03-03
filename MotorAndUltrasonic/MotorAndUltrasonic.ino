/*
HC-SR04 Ping distance sensor
*/
/*
 * Conditions for wheel motors:
 *    Outriggers are down 
 *    L1 is held down
 *    No obstacle in direction
 *
 *  There were some problems with the sensor execution along with Bluetooth communication. Without replacing the sensors, there are two proposed solutions:
 *  1) Use a slave Arduino unit with the sensor processing, and send the output of the sensors (tell whether or not an obstacle is near) to the master Arduino.
 *  2) Use asynchronous procedures for the sensors. Use a master interrupt flag which asynchronously allows the MOCL to move if an obstacle is detected.
*/
#include <Sabertooth.h>
//PS3 Wired USB
#include <PS3USB.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif
const int LED = 2;
Sabertooth ST(128);              //first motor drive
Sabertooth ST2(129);             //check second motor drive alternative address
USB Usb;
PS3USB PS3(&Usb);                // This will just create the instance
int pow1, pow2, pow3, pow4;      // The individual power levels of each motor
 long duration, inches, cm;
 long duration2, duration3, duration4;
  //Designated speeds
  const int slowm = 40;     //speed of MOCL when obstacle is close
  const int maximum = 127;  //fastest speed of motors
  const int stepPow = 10;   //amount by which MOCL decelerates
  //Warning Zone
  const int warni = 12;     //how far away is obstacle for MOCL to slow down
 //Stop Zone
  const int avoi = 6;       //how far away is obstacle for MOCL to stop
  //IR Sensors
const int IR1 = 13;
const int IR2 = 12;
const int IR3 = 11;
const int Light = 10;
  //Motor Drive Signals
  //Sabertooth requires TX1 Line by default. Connect all signal lines of Motor Drives to TX1 on Arduino
//const int Mo1 = 14;
//const int Mo3 = 17;
//const int Mo5 = 18;
  //Ultrasonic
const int USt1 = 22;
const int USe1 = 23;
const int USt2 = 24;
const int USe2 = 25;
const int USt3 = 26;
const int USe3 = 27;
const int USt4 = 28;
const int USe4 = 29;
const int USt5 = 30;
const int USe5 = 31;
const int USt6 = 32;
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
const int USt13 = 46;
const int USe13 = 47;
const int USt14 = 48;
const int USe14 = 49;

  void Output() {
    pinMode(LED, OUTPUT);
    //Front Transmitters
    pinMode(USt1, OUTPUT);
    pinMode(USt2, OUTPUT);
    pinMode(USt3, OUTPUT);
    pinMode(USt4, OUTPUT);

    pinMode(USt5, OUTPUT);
    pinMode(USt6, OUTPUT);
    pinMode(USt7, OUTPUT);
    pinMode(USt8, OUTPUT);
    pinMode(USt9, OUTPUT);
    pinMode(USt10, OUTPUT);
    pinMode(USt11, OUTPUT);
    pinMode(USt12, OUTPUT);
    pinMode(USt13, OUTPUT);
    pinMode(USt14, OUTPUT);

  }

    void Input () {
      //Front Echoes
    pinMode(USe1, INPUT);
    pinMode(USe2, INPUT);
    pinMode(USe3, INPUT);
    pinMode(USe4, INPUT);

    pinMode(USe5, INPUT);
    pinMode(USe6, INPUT);
    pinMode(USe7, INPUT);
    pinMode(USe8, INPUT);
    pinMode(USe9, INPUT);
    pinMode(USe10, INPUT);
    pinMode(USe11, INPUT);
    pinMode(USe12, INPUT);
    pinMode(USe13, INPUT);
    pinMode(USe14, INPUT);

  }

  void PS3setup()
  {
    #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
    if (Usb.Init() == -1) {
      Serial.print(F("\r\nOSC did not start"));
      while (1) {             //halt
      digitalWrite(13, LOW);
      delay(1200);
      digitalWrite(13, HIGH);
      delay(1200);
      }
    }
    Serial.println(F("\r\nPS3 USB Library Started"));
  }

  int inCrease(int powmo) {
    if (powmo < maximum)  {
      powmo ++;
    }
    return powmo;
  }

  int deCrease(int powmo)  {
    if (powmo > (-1*maximum))  {
      powmo --;  
    }
    return powmo;
  }
         
  int dropDown(int powmo) {
    if ((powmo < -10) || (powmo > 10))  {
      if (powmo < 0)  {
        powmo+= stepPow;
      }
      if (powmo > 0)  {
        powmo-= stepPow;
      }
      else  {
        if (powmo > 0)  {
          powmo--;
        }
        if (powmo < 0)  {
          powmo++;
        }
      }
    }
  return powmo;
  }

  int slowDown(int powmo) {
    if ((powmo < -10) || (powmo > 10))  {
      if (powmo < 0)  {
        powmo+= stepPow;
      }
      if (powmo > 0)  {
        powmo-= stepPow;
      }
      else  {
        if (powmo > 0)  {
          powmo--;
        }
        if (powmo < 0)  {
          powmo++;
        }
      }
    }
  return powmo;
  }

  void PS3leftControl()
  {
    if (PS3.PS3Connected) {             // Sticks influence motor power
        if (PS3.getButtonPress(L1)) {   // Button needs to be held
            //Forward
            if (PS3.getAnalogHat(LeftHatY) < 50) {
              inCrease(pow1);
              inCrease(pow2);
              inCrease(pow3);
              inCrease(pow4);
              }
            //Backward
            if (PS3.getAnalogHat(LeftHatY) > 200) {
              deCrease(pow1);
              deCrease(pow2);
              deCrease(pow3);
              deCrease(pow4);
              }
            //Left
            //Right
            }
        }
    }
  
  void ping()
  {
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
    digitalWrite(USt13, LOW);  
    digitalWrite(USt14, LOW); 
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
    digitalWrite(USt13, HIGH);  
    digitalWrite(USt14, HIGH);
    delayMicroseconds(10); 
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
    digitalWrite(USt13, LOW);  
    digitalWrite(USt14, LOW);
  }

void warning (int dist)
{
  if (dist <= warni)
  {
    pow1 = slowDown(pow1);
    pow2 = slowDown(pow2);
    pow3 = slowDown(pow3);
    pow4 = slowDown(pow4);
  }
}

void stopping (int dist)
{
  if (dist <= avoi)
  {
    pow1 = dropDown(pow1);
    pow2 = dropDown(pow1);
    pow3 = dropDown(pow1);
    pow4 = dropDown(pow1);
  }  
}
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return (microseconds / 74 / 2);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


long durationAverage(long d1, long d2, long d3, long d4)
{
  int avg = (d1+d2+d3+d4) / 4;
  return avg;
}

void setup() 
{
  Serial.begin (115200);
//  SoftwareSerial.begin();
  SabertoothTXPinSerial.begin(115200);
  pow1 = 0; pow2 = 0; pow3 = 0; pow4 = 0;
  ST.autobaud();
  PS3setup();
  Output();
  Input();
}

void loop() 
{
  Usb.Task();
  ping();
  duration = pulseIn(USe1, HIGH);
  duration2 = pulseIn(USe2, HIGH);
  duration3 = pulseIn(USe3, HIGH);
  duration4 = pulseIn(USe4, HIGH);
  duration = durationAverage(duration, duration2, duration3, duration4);
  inches = microsecondsToInches(duration);

  warning(inches);
  stopping(inches);
//  resuming(inches);
//Serial.println(power);
  ST.motor(1, pow1);   //As all motors share power, they all move in the same direction and speed
  ST.motor(2, pow2);
  ST2.motor(1, pow3);
  ST2.motor(2, pow4);
  delay(50);
}   // END OF void loop()
