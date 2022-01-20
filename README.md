# Q-Sys-Remote-GPIO-Nano
This is a Q-Sys plugin and associated Arduino code to create a low cost open source network GPIO peripheral for the Q-Sys architecture.


The Remote GPIO Nano is a 5-volt tolerant network GPIO peripheral designed for the Q-SYS architecture. The peripheral has a total of 16 fully configurable GPIO pins, 8 digital IO and 8 analog inputs (6 of which can be configured as additional digital IO). Built around readily available open-source hardware the Remote GPIO Nano can be assembled for around $25 per unit. An Arduino Nano microcontroller in companion with an ENC28J60 ethernet shield enables UDP communication with the Q-SYS core. This plugin allows for ease of setup and integration into an existing design appearing as any other configurable GPIO component. 


HARDWARE

Arduino Nano

enc28j60 Arduino Ethernet Shield

Nano Terminal Adapter Shield


SETUP

After installing the Arduino IDE and connecting the Arduino Nano via USB to the PC you can start by opening the Remote-GPIO-Nano-Config file. The Config file allows you to modify the default static IP address, choose between a Static or DHCP IP, select the UDP communication port, and set the device Hostname. Editing of the Config file is optional as all these settings can later be changed through the web UI accessible though Q-Sys configurator. Once ready the Config file can then be uploaded to the Arduino, this Config file will only need to be uploaded once onto a new Arduino board as it creates the necessary framework for the default settings in the eeprom of the Arduino. After the Config file has been successfully uploaded to the board you can then open the Q-SYS-Remote-GPIO-NanoV1 file in the Arduino IDE, simply upload this file to the Arduino board. Congratulations you have now completed the most difficult part of the setup. Now for the easy part of setting up the hardware. Depending on where you sourced the hardware from you may need to solder the header pins onto the Arduino and ethernet shield, but they typically come presoldered making setup much quicker. With the pins attached the Arduino Nano will plug into the top of the ethernet shield and the ethernet shield will then plug into the terminal breakout board. Lastly plug in the ethernet cable and connect 5v power to the board via a micro USB cable and USB power supply or a 5v power supply connected to the screw terminals of the breakout board. This completes the Arduino portion of the setup and the hardware will be ready to go.

After installing the Q-Sys plugin drag the component into the design and configure it as any other GPIO component. Once the design is running the IP Address and UDP port can be entered in the Setup/Status tab. To find the IP Address and UDP port after the hardware has been setup and plugged in navigate to Q-Sys Configurator and look under I/O Devices > I/O-Frame8S and you should see a device named "ArduinoNano" click on the device and select the "Open Peripheral Manager" link in the right side pane. *If there is an error where Configurator shows the IP Address as 0.0.0.0 press the reset button on the Arduino and it should fix the problem. This should open a web browser to the IP Address of the Remote GPIO Nano, this is where you can now change the settings from before in the simple web UI. Once the settings have been changed press submit and the Remote GPIO Nano will reset and temporarily drop off the network as the new settings take effect.
