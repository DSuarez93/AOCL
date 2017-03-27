/*  This is a collection of different methods of controlling the motors.
    Use these if you want to have gradual or conditional changes to motors.
    These work better than discrete values.
    I did not test any of these, so test with caution, then update this.
*/

//  Do you want to adjust speed quickly? Change to 1 if No.
#define steady 0;
const int quickly 10;

const int[] rate[ 2, 5, 10, 18, 30];
int rateStep;

void initStep() {
  rateStep = 0;
}

/*  Have the motors steadily decelerate back to stop moving. 
    This works by getting the input variable slowly back to 0.
    Compensate for forwards or backwards movement.
*/  
int neutralSteadily(int var) {

  if ( steady == 0 && ( (var > -quickly) || (var < quickly)) ) {
    var = neutralQuickly(var);
  }  
  else {
    if (var < 0)
      var++;
    if (var > 0)
      var--;
  }
  return var;
}

int neutralQuickly(int var) {

  if (var < 0)
    var+=quickly;
  if (var > 0)
    var-=quickly;
  return var;
}

/*  Use this to change the maximum speed if there is a weight detected on the 
    Input should be the maximum speed
*/
int increaseMaxIfWeight(int var, bool forward) {
 
 if (motorSensor) {
    if (forward)
      var+=20;
    else
      var-=20;
  }
  return var;
}
