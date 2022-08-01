#include "RunningAverage.h" //download this library on Arduino

RunningAverage myRA(10); //include these 2 lines of code before void setup()
int samples = 0;

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Demo RunningAverage lib");
  Serial.print("Version: ");
  Serial.println(RUNNINGAVERAGE_LIB_VERSION);
  myRA.clear(); // explicitly start clean -- ONLY need this line in void setup()
}

void loop(void) 
{
  long rn = random(0, 1000); //this line initiates the variable, change "random(0,1000)" to the variable name you want to take the running average of; long is the type of variable rn-- running average-- is 
  myRA.addValue(rn * 0.001);
  samples++;
  Serial.print("Running Average: ");
  Serial.println(myRA.getAverage(), 3);

  if (samples == 300) // this line sets the number of samples included in the running average
  {
    samples = 0;
    myRA.clear();
  }
  delay(100); //this line indicates the delay, lower the delay to collect more data points per time
}
