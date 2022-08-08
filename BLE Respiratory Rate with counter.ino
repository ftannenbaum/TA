//Unsuccessful code-- only connects with bluetooth after inputting upper and lower limits on Serial Monitor. After inputting upper and lower limits the board connects with Light Blue and can be written to and read

// Libraries
#include "PDM.h"
#include "arduinoFFT.h"
#include "RunningAverage.h"
#include "ArduinoBLE.h"

// Definitions
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 16000 //Hz, must be less than 10000 due to ADC
#define samp_siz 4 // Experiment
#define rise_threshold 4 // Experiment 

short sampleBuffer[SAMPLES];
volatile int samplesRead;

unsigned long microseconds;


RunningAverage myRA(10); //include these 2 lines of code before void setup()
int samples = 0;
int count = 0;
int data;
bool inPeak = false;

int x = 0;
int LastTime = 0;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;
#define UpperThreshold 2500
#define LowerThreshold 1700
int LED13 = 44; // The on-board Arduino LED
int Signal; // holds the incoming raw data. Signal value can range from 0-1024

double vReal[SAMPLES];
double vImag[SAMPLES];

void onPDMdata(void);

const uint8_t amplitude = 100;

arduinoFFT FFT = arduinoFFT();

// Variables for Limits
int UpperLimit;
int LowerLimit;
int counter;
int gate;

// Bluetooth
BLEService echoService("00000000-0000-1000-8000-00805f9b34fb");
BLEStringCharacteristic charac ("741c12b9-e13c-4992-8a5e-fce46dec0bff", BLERead | BLEWrite | BLENotify, 40);
BLEDescriptor Descriptor("beca6057-955c-4f8a-e1e3-56a1633f04b1", "Descriptor");
String var = "";

void setup() {
  gate = 3;
  pinMode(LED_BUILTIN, OUTPUT); //setup the LED light
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Bluetooth setup
  if (!BLE.begin()) {
    Serial.println("starting BLE failed.");
    while (1);
  }

  BLE.setLocalName("Arduino BLE Echo");
  BLE.setAdvertisedService(echoService);
  charac.addDescriptor(Descriptor);
  echoService.addCharacteristic(charac);
  BLE.addService(echoService);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
  Serial.println(" ");

 if (gate > 2) {
    Serial.println("////////// TrachAlert System //////////");
    Serial.println("What should the upper limit for the respiratory rate be?");
    while (Serial.available() == 0) {
      // Wait for User to Input Data
    }
    UpperLimit = Serial.parseInt(); //Read the data the user has input
    Serial.print("Upper respiratory rate limit set to: ");
    Serial.print(UpperLimit);
    Serial.println(" BPM");
    Serial.println("And what should the lower limit for the respiratory rate be?");
    while (Serial.available() == 1) {
      // Wait for User to Input Data
    }
    LowerLimit = Serial.parseInt(); //Read the data the user has input
    Serial.print("Lower respiratory rate limit set to: ");
    Serial.print(LowerLimit);
    Serial.println(" BPM");
    Serial.println("Thank you for choosing TrachAlert. Respiratory rate measurement will start in 5 seconds.");
    counter = 1000; // Initial counter value
    delay(5000); // Seconds before respiratory rate measurement begins
    gate = 1;
  }

  PDM.onReceive(onPDMdata);
  PDM.setBufferSize(SAMPLES);
  //PDM.setGain(0);
  // setup the I2S audio input for the sample rate with 32-bits per sample
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
  Serial.begin(115200); // Original = 115200
  myRA.clear(); // explicitly start clean -- ONLY need this line in void setup()
}

void loop() {
 //Bluetooth connection loop
  BLEDevice central = BLE.central();
  if(central){
    Serial.println("* Connected to central device!");
    Serial.print("Connected to central : ");
    Serial.println(central.address());
    Serial.println(" ");

    while(central.connected()){ //set Data format on app to UTF-8 String
      if(charac.written()){
        var = charac.value();
        Serial.println(String(var));
        delay(500);
        charac.writeValue(var);
        Serial.println("write stringCharacteristic");
      }
      
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());   
  }
  
 
  if (samplesRead) {
    for (int i = 0; i < SAMPLES; i++) {
      vReal[i] = sampleBuffer[i];
      vImag[i] = 0;
    }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    //Serial.println(peak);
    double rn = peak ; //this line initiates the variable, change "random(0,1000)" to the variable name you want to take the running average of; long is the type of variable rn-- running average-- is
    myRA.addValue(rn);
    samples++;
    Serial.print("Running Average: ");
    Serial.println(myRA.getAverage(), 4);

    if (samples == 4000) // this line sets the number of samples included in the running average
    {
      samples = 0;
      myRA.clear();
    }
    samplesRead = 0;
    //delay(80);

    int value = myRA.getAverage();
    if (value > UpperThreshold) {
      if (BeatComplete) {
        BPM = millis() - LastTime;
        BPM = int(60 / (float(BPM) / 1000));
        BPMTiming = false;
        BeatComplete = false;
      }
      if (BPMTiming == false) {
        LastTime = millis();
        BPMTiming = true;
      }
    }
    if ((value < LowerThreshold) & (BPMTiming))
      BeatComplete = true;
    // display bpm
    Serial.print(BPM);
    Serial.println(" BPM");

    if (BPM > UpperLimit || BPM < LowerLimit) { // If the BPM exceeds the upper limit or drops below the lower limit...
      counter = counter - 1;                   // Subtract 1 from the counter value.
    } else {                                   // Otherwise....
      counter = 300;                           // Reset the counter value to the specified number...
      digitalWrite(LED_BUILTIN, HIGH);         // And turn the LED off.
    }
    if (counter <= 0) {                               // If the counter value reaches 0 or below...
      digitalWrite(LED_BUILTIN, LOW);                 // Turn the LED on...
      Serial.println("Abnormal Breathing Detected!"); // And alert the doctor of the abnormal breathing pattern.
    }

    x++;
    Signal = myRA.getAverage(); // Read the PulseSensor's value.
    // Assign this value to the "Signal" variable.
  }
}

void onPDMdata()
{
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
