/***
    eeprom_put example.

    This shows how to use the EEPROM.put() method.
    Also, this sketch will pre-set the EEPROM data for the
    example sketch eeprom_get.

    Note, unlike the single byte version EEPROM.write(),
    the put method will use update semantics. As in a byte
    will only be written to the EEPROM if the data is actually
    different.

    Written by Christopher Andrews 2015
    Released under MIT licence.
***/

#include <EEPROM.h>



void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //One simple call, with the address first and the object second.
  EEPROM.put(10, 10);   // IP Address first octet
  EEPROM.put(20, 1);    // IP Address second octet
  EEPROM.put(30, 30);   // IP Address third octet
  EEPROM.put(40, 23);   // IP Address fourth octet
  EEPROM.put(50, "DHCP");     // Set a DHCP or Static IP Address
  EEPROM.put(60, 8888);     // Set the UDP port
  EEPROM.put(100, "ArduinoNano");   // Set the device Hostname

  Serial.println("Written data to EEPROM");

}

void loop() {
  /* Empty loop */
}
