 /*
 * Motor Driver
 */
const int Mo1 = 17;
const int Mo2 = 18;
const int Mo3 = 14;
SoftwareSerial SWSerial(NOT_A_PIN, Mo1);
SoftwareSerial SWSerial2(NOT_A_PIN, Mo2);
SoftwareSerial SWSerial3(NOT_A_PIN, Mo3);
Sabertooth ST(128, SWSerial);
Sabertooth ST2(129, SWSerial2);
Sabertooth ST3(130, SWSerial3);               //third motor needs a different address
int pow1, pow2, pow3, pow4, pow5;          
//pow# with designated motor
int maxp;                    //127 is the fastest a motor can go
#define tops 32              //absolute top motor speed
const int wane = 10;         //Transition speed
const int scis = 127;        //For Scissor Lift Drive, 127 is stop
const int scid = 197;        //197 - 127 = 70     Go Down
const int sciu = 3;          //127 - 3 = 124      Go Up
/*
    Front of MOCL
  Mo2.1         Mo1.1


  Mo2.2         Mo1.2
*/

void motorSetup() {
    pinMode(Mo1, OUTPUT);
    pinMode(Mo2, OUTPUT);
    pinMode(Mo3, OUTPUT);
    delay(2000);                             //allow two seconds between power and baud character sent to lift motor
    SWSerial.begin(38400);                   
    SWSerial2.begin(38400);                  
    SWSerial3.begin(38400);                  //Must be 38400 which is the fastest baud rate of Sabertooth 50
    SabertoothTXPinSerial.begin(38400);      
    maxp = tops;
    pow1 = 0; pow2 = 0; pow3 = 0; pow4 = 0; pow5 = 0;
    ST.setBaudRate(38400);
    ST2.setBaudRate(38400);                  //Lift motor drive accepts single byte commands, baud set on DIP switches on motor drive itself
}

int coast(int variable)  {
  //If the value is large, go back to zero quickly.
    if ((variable < -10) || (variable > 10))  {
      if (variable < 0)  {
        variable+=5;
      }
      if (variable > 0)  {
        variable-=5;
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

int boundaryCheck(int variable)  {
    if (maxp == 0) {
      return 0;
    }   
    if (variable > maxp) {
      return maxp-1;
    }
    if (variable < -maxp) {
      return -maxp+1;
    }
}
