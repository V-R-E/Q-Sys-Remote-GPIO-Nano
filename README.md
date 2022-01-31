# Q-Sys-Remote-GPIO-Nano
This is a Q-Sys plugin and associated Arduino code to create a low cost open source network GPIO peripheral for the Q-Sys architecture.

## Overview
The Remote GPIO Nano is a 5-volt tolerant CMOS network GPIO peripheral designed for the Q-SYS architecture. The peripheral has a total of 16 fully configurable GPIO pins, 8 digital IO and 8 analog inputs (6 of which can be configured as additional digital IO). Built around readily available open-source hardware the Remote GPIO Nano can be assembled for around $25 per unit. An Arduino Nano microcontroller in companion with an ENC28J60 ethernet shield enables UDP communication with the Q-SYS core. This plugin allows for ease of setup and integration into an existing design appearing as any other configurable GPIO component. 

## Properties
- Show Digital Pins
  - *Exposes digital pins 1-8 for configuration*
- Show Analog Pins
  - *Exposes analog pins 1-8 for configuration*
- GPIO D1-8
  - *Configures GPIO pin as **Unused,[^1] Digital Input,[^2] Digital Output[^3]***
- GPIO A1-6
  - *Configures GPIO pin as **Unused,[^1] Analog Input,[^4] Digital Input,[^2] A/D Input,[^5] Digital Output[^3]***
- GPIO A7-8
  - *Configures GPIO pin as **Unused,[^1] Analog Input,[^3] Digital Input,[^2] A/D Input,***

[^1]: The GPIO pin is not used.
[^2]: The GPIO pin is a CMOS Input (0v-1.5v = False)(3.5v-5v = True)
[^3]: The GPIO pin is a CMOS Output (False = 0v)(True = 5v)
[^4]: The GPIO pin is a 5v referenced Analog Input (0v-5v = 0-1023).
[^5]: The GPIO pin is a hybrid analog digital pin. The analog value is output along with an equivalent digital output state. (0v-5v = 0-1023)/(0-512 = False)(513-1023 = True)


## Control Pin Definitions
- Digital 1…16 (The available Control Pins depend on settings in Properties)
  - *Digital Input: **Read Only*** [1,0]
  - *Digital Output: **Write Only*** [1,0]
- Analog 1…8 (The available Control Pins depend on settings in Properties)
  - *Analog Input,: **Read Only*** [0-1023]
  - *A/D Input: **Read Only*** [0-1023 / 1,0]
- Analog Reference Voltage (Voltage use to refence the analog pins against. *Should not exceed 5v)
  - ***Read Only*** [0.00v-5.50v]
- Device IP (IP Address of The Remote GPIO Nano)
  - ***Read, Write***
- Device Port (UDP port the The Remote GPIO Nano is listening on)
  - ***Read, Write***
- Status (Status of The Remote GPIO Nano connection)
  - ***Read Only***
- Up Time (Up Time of the The Remote GPIO Nano)
  - ***Read Only***

## HARDWARE

- Arduino Nano
  
- enc28j60 Arduino Ethernet Shield
  
- Nano Terminal Adapter Shield

## SETUP

*This assumes that you are familiar with how to program an Arduino and install corresponding libraries.*

After installing the Arduino IDE and connecting the Arduino Nano via USB to the PC you can start by opening the Remote-GPIO-Nano-Config file. The Config file allows you to modify the default static IP address, choose between a Static or DHCP IP, select the UDP communication port, and set the device Hostname. Editing of the Config file is optional as all these settings can later be changed through the web UI accessible though Q-Sys configurator. Once ready the Config file can then be uploaded to the Arduino, this Config file will only need to be uploaded once onto a new Arduino board as it creates the necessary framework for the default settings in the eeprom of the Arduino. After the Config file has been successfully uploaded to the board you can then open the Q-SYS-Remote-GPIO-NanoV1 file in the Arduino IDE, simply upload this file to the Arduino board. Congratulations you have now completed the most difficult part of the setup. Now for the easy part of setting up the hardware. Depending on where you sourced the hardware from you may need to solder the header pins onto the Arduino and ethernet shield, but they typically come presoldered making setup much quicker. With the pins attached the Arduino Nano will plug into the top of the ethernet shield and the ethernet shield will then plug into the terminal breakout board. Lastly plug in the ethernet cable and connect 5v power to the board via a micro-USB cable and USB power supply or a 5v power supply connected to the screw terminals of the breakout board. This completes the Arduino portion of the setup, and the hardware will be ready to go.

  After installing the Q-Sys plugin drag the component into the design and configure it as any other GPIO component. Once the design is running the IP Address and UDP port can be entered in the Setup/Status tab. To find the IP Address and UDP port after the hardware has been setup and plugged in navigate to Q-Sys Configurator and look under I/O Devices > I/O-Frame8S and you should see a device named "ArduinoNano" click on the device and select the "Open Peripheral Manager" link in the right-side pane. *If there is an error where Configurator shows the IP Address as 0.0.0.0 press the reset button on the Arduino and it should fix the problem. This should open a web browser to the IP Address of the Remote GPIO Nano, this is where you can now change the settings from before in the simple web UI. Once the settings have been changed press submit and the Remote GPIO Nano will reset and temporarily drop off the network as the new settings take effect.
