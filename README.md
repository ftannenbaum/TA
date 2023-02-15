# TA
## 7/19 Update: Edge Impulse guidance

Upload Edge Impulse code to board before launching PowerShell. Launch PowerShell and type "edge-impulse-data-forwarder" (if needed, frequency can be overwritten by typing "edge-impulse-data-forwarder frequency __" with desired frequency in Hz). Select which project you are connecting the device to. Name the two sensor axes- for this instance you can name them "noise,temp". Name board to be identified on EI "XIAO BLE SENSE". Once connected, follow EI steps as normal

Troubleshooting:

If output says invalid API key: run "edge-impulse-data-forwarder --clean" and then redo steps to log in 

Note: Frequency needs to be >6Hz for Edge Impulse to run properly. If frequency is being detected but is defaulting back to the last used frequency without being overwritten, type "edge-impulse-data-forwarder --clean" and redo steps to log in. Steps to change frequency to desired Hz can be found in comments at top of EI code



## 7/25 update:

Used "XiaoMic_Freq_UsedforEI.ino" code to gather data on Edge Impulse. Data was only collected for 75% blockage and 0% blockage to make it easier to distinguish as opposed to using 0%, 25%, 50%, and 75%. Accuracy was 55% and the Arduino zip library was downloaded: [ei-freq_test-arduino-1.0.1.zip](https://github.com/ftannenbaum/TA/files/9183898/ei-freq_test-arduino-1.0.1.zip). 

Note: "XIAO-motion.ino" has the library for the left-right and up-down classifications as well as the corresponding written code to call upon the library.


## 2/15 Update:
Thermistor and board information:

https://learn.adafruit.com/thermistor/using-a-thermistor

https://www.seeedstudio.com/Seeed-XIAO-BLE-Sense-nRF52840-p-5253.html

https://wiki.seeedstudio.com/XIAO_BLE/

## Helpful links:

[How to use arrays](https://docs.arduino.cc/built-in-examples/control-structures/Arrays)

[How to use running average](https://playground.arduino.cc/Main/RunningAverage/)

[How to use over/under temperature to find respiratory rate](https://forum.arduino.cc/t/measuring-respiratory-rate-with-temperature-sensor/702183)

[How to use temperature rolling average to get breath rate](https://forum.arduino.cc/t/respiration-rate-with-a-temperature-sensor-arduino/943352/4)

[How to use 'millis' for timing on Arduino](https://forum.arduino.cc/t/using-millis-for-timing-a-beginners-guide/483573)

[Idea on how to write to board from LightBlue](https://compasstech.com.au/arduino/ard2.html)



