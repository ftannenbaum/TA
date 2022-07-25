# TA
7/19 Update: Edge Impulse guidance

Upload Edge Impulse code to board before launching PowerShell. Launch PowerShell and type "edge-impulse-data-forwarder" (if needed, frequency can be overwritten by typing "edge-impulse-data-forwarder frequency __" with desired frequency in Hz). Select which project you are connecting the device to. Name the two sensor axes- for this instance you can name them "noise,temp". Name board to be identified on EI "XIAO BLE SENSE". Once connected, follow EI steps as normal

Troubleshooting:

If output says invalid API key: run "edge-impulse-data-forwarder --clean" and then redo steps to log in 

Note: Frequency needs to be >6Hz for Edge Impulse to run properly. If frequency is being detected but is defaulting back to the last used frequency without being overwritten, type "edge-impulse-data-forwarder --clean" and redo steps to log in. Steps to change frequency to desired Hz can be found in comments at top of EI code



7/25 update:

Used "XiaoMic_Freq_UsedforEI.ino" code to gather data on Edge Impulse. Data was only collected for 75% blockage and 0% blockage to make it easier to distinguish as opposed to using 0%, 25%, 50%, and 75%. Accuracy was 55% and the Arduino zip library was downloaded: [ei-freq_test-arduino-1.0.1.zip](https://github.com/ftannenbaum/TA/files/9183898/ei-freq_test-arduino-1.0.1.zip). 

Note: "XIAO-motion.ino" has the library for the left-right and up-down classifications as well as the corresponding written code to call upon the library.

Looking forward:
Tomorrow we will try Edge Impulse with the audio recognition feature as opposed to the "something else" sensor type
