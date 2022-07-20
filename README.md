# TA
7/19 Update:
Upload EI code to board before launching PowerShell

Launch PowerShell and type "edge-impulse-data-forwarder" (if needed, frequency can be overwritten by typing "edge-impulse-data-forwarder frequency __" with desired frequency in Hz)

Select which project you are connecting the device to

Name the two sensor axes- for this instance you can name them "noise,temp"

Name board to be identified on EI "XIAO BLE SENSE"

Once connected, follow EI steps as normal

Troubleshooting:
If output says invalid API key: run "edge-impulse-data-forwarder --clean" and then redo steps to log in 
