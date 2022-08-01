#include <RunningAverage.h>
#include <PDM.h>
#include <arduinoFFT.h>


#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 16000 //Hz, must be less than 10000 due to ADC
short sampleBuffer[SAMPLES];
volatile int samplesRead;

unsigned long microseconds;


double vReal[SAMPLES];
double vImag[SAMPLES];

void onPDMdata(void);

const uint8_t amplitude = 100;

arduinoFFT FFT = arduinoFFT();

RunningAverage myRA(10); //for the running average
int samples = 0;

void setup() {
  Serial.begin(115200);
  
  myRA.clear(); // explicitly start running average clean
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  PDM.onReceive(onPDMdata);
  PDM.setBufferSize(SAMPLES);
  //PDM.setGain(0);
  // setup the I2S audio input for the sample rate with 32-bits per sample
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop() {
  if (samplesRead) {
    for (int i = 0; i < SAMPLES; i++) {
      vReal[i] = sampleBuffer[i];
      vImag[i] = 0;
    }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
 
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    
  //Serial.println(peak);   //prints all frequency values, even background noise and voices
    
    samplesRead = 0;

   if (peak < 1120){     //gets rid of some background noise and voices, prints 0 instead of the actual frequency
      Serial.println(0);
    }
    else {
      Serial.println(peak);
    }
    
    
    
    long rn = peak;
    myRA.addValue(rn * 0.001);
    samples++;
    Serial.print("Running Average: ");
    Serial.println(myRA.getAverage(), 3); //prints the running average after each frequency that is printed

    if (samples == 300)
    {
     samples = 0;
      myRA.clear();
    }
     delay(100);
  
    if (myRA.getAverage() == 0) 
    {
      Serial.println ("Alert: Respiratory rate = 0");
    }

    if (myRA.getAverage() > 1.0)
    {
      Serial.println ("Breath");
    }
}

void onPDMdata()
{
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;

}
