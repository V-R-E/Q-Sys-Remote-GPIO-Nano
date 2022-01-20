


#include <SPI.h>         // needed for Arduino versions later than 0018
//#include <Ethernet.h>        // Ethernet library for offical w5500 shields
#include <EthernetENC.h>        // Ethernet library for cheap enc28j60 shields
#include <EthernetUdp.h>        // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <EEPROM.h>







// Enter a MAC address for your Remote GPIO Nano.
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x23 };


// The IP address will be dependent on your local network:  
//IPAddress ip(10, 1, 30, 23);      // static IP address for your Remote GPIO Nano.
//unsigned int localPort = 6504;      // local port for the Remote GPIO Nano to listen on

// The IP address will be dependent on your local network:  
//IPAddress ip(192,168,0,23);      // static IP address for your Remote GPIO Nano.
//unsigned int localPort = 6504;      // local port for the Remote GPIO Nano to listen on


// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
#define W5200_CS  10

////// The IP address will be dependent on your local network:  
//IPAddress remoteIP(255, 255, 255, 255);     // IP of the controller (Q-SYS Core)
//unsigned int remotePort = 2467;       // local port the controller (Q-SYS Core) is listening on

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// The broadcast IP address:  
IPAddress Bip(255, 255, 255, 255);     // IP of the controller (Q-SYS Core)

int period = 900;
unsigned long time_now = 0;

int IDpin = 1;    // choose the pin for the ID LED
int PinNum = 2;   // Var for storing current IO pin for updating outputs
int ipA,ipB,ipC,ipD;    // Vars for storing IP address
int ipAget,ipBget,ipCget,ipDget;    // Vars for storing IP address
int Port;    // Var for storing port address


bool sel = false;


String N;

char StaticMode[7];    // Var for storing Static or DHCP mode
char HostName[21];   // Var for storing device hostname
char numBox[26] = " <input type=number name=";
char brk[9] = "<br><br>";
char ipRange[22] = " min=0 max=255 value=";
char sendPacket = "";

// Parsing vars
char * QsysData; 
char EndDelimiter = '*';
char Delimiter2 = ':';





long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}





void setup() {

   pinMode(1, OUTPUT);      // sets the digital pin as output
   pinMode(0, INPUT_PULLUP);      // sets the digital pin as output
   pinMode(2, INPUT);      // sets the digital pin as input
   pinMode(3, INPUT);      // sets the digital pin as input
   pinMode(4, INPUT);      // sets the digital pin as input
   pinMode(5, INPUT);      // sets the digital pin as input
   pinMode(6, INPUT);      // sets the digital pin as input
   pinMode(7, INPUT);      // sets the digital pin as input
   pinMode(8, INPUT);      // sets the digital pin as input
   pinMode(9, INPUT);      // sets the digital pin as input

//   pinMode(A0, INPUT_PULLUP);
//   pinMode(A1, INPUT_PULLUP);
//   pinMode(A2, INPUT_PULLUP);
//   pinMode(A3, INPUT_PULLUP);
//   pinMode(A4, INPUT_PULLUP);
//   pinMode(A5, INPUT_PULLUP);

//   digitalWrite(2, HIGH);     // Set pins high at boot
//   digitalWrite(3, HIGH);
//   digitalWrite(4, HIGH);
//   digitalWrite(5, HIGH);
//   digitalWrite(6, HIGH);
//   digitalWrite(7, HIGH);
//   digitalWrite(8, HIGH);
//   digitalWrite(9, HIGH);

//   digitalWrite(2, LOW);      // Set pins low at boot
//   digitalWrite(3, LOW);
//   digitalWrite(4, LOW);
//   digitalWrite(5, LOW);
//   digitalWrite(6, LOW);
//   digitalWrite(7, LOW);
//   digitalWrite(8, LOW);
//   digitalWrite(9, LOW);



    EEPROM.get(10, ipAget);   // Get the saved IP[0] value from the eeprom
    EEPROM.get(20, ipBget);   // Get the saved IP[1] value from the eeprom
    EEPROM.get(30, ipCget);   // Get the saved IP[2] value from the eeprom
    EEPROM.get(40, ipDget);   // Get the saved IP[3] value from the eeprom
    EEPROM.get(50, StaticMode);   // Get the saved Static or DHCP state from the eeprom
    EEPROM.get(60, Port);   // Get the saved port value from the eeprom
    EEPROM.get(100, HostName);   // Get the saved Hostname from the eeprom

//    // The IP address will be dependent on your local network:  
//    IPAddress ip(A, B, C, D);      // static IP address for your Remote GPIO Nano.
//    unsigned int localPort = 6504;      // local port for the Remote GPIO Nano to listen on

  IPAddress ip(ipAget, ipBget, ipCget, ipDget);
  unsigned int localPort = Port;      // local port for the Remote GPIO Nano to listen on
  
  // start the Ethernet and UDP:
  if(strncmp(StaticMode,"Static",6) == 0) {
    Ethernet.begin(mac,ip);
    sel = false;
  }
  else {
    Ethernet.begin(mac);
    sel = true;
  }


//  Serial.begin(115200);
//
//  Serial.println(Ethernet.localIP());
  

  Udp.begin(localPort);

    // start the server
  server.begin();

 
}



void(* resetFunc) (void) = 0; //declare reset function @ address 0





void loop() {
        
        
        if(millis() >= time_now + period){
        time_now += period;
          Udp.beginPacket(Bip, 2467);
          Udp.write("<QDP><device>\r  <name>");
          Udp.write(HostName);
          Udp.write("</name>\r  <type>ioframe8s</type>\r  <lan_a_ip>");
          Udp.print(Ethernet.localIP());
          Udp.write("</lan_a_ip>\r <periph_cfg_url>/#network</periph_cfg_url>\r</device>\r</QDP>");
          Udp.endPacket();
      }

  

  
 //========== Read incoming UDP packets ===============================
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    
////********** Uncomment below for Debugging ******************************
//    Serial.print("Received packet of size ");
//    Serial.println(packetSize);
//    Serial.print("From ");
//    IPAddress remote = Udp.remoteIP();
//    for (int i =0; i < 4; i++)
//    {
//      Serial.print(remote[i], DEC);
//      if (i < 3)
//      {
//        Serial.print(".");
//      }
//    }
//    Serial.print(", port ");
//    Serial.println(Udp.remotePort());
////********** Uncomment above for Debugging ******************************

    float vccValue=readVcc()/1000.0;  //Arduino refence voltage
    char packetBuffer[packetSize];  // buffer to hold incoming packet

    // read the packet into packetBufffer
    Udp.read(packetBuffer, packetSize);
//    Serial.println("Contents:");
//    Serial.println(packetBuffer);


//========== Parse the things ===============================
    
    QsysData = strtok(packetBuffer, &EndDelimiter);
    QsysData = strtok(packetBuffer, &Delimiter2);
//    Serial.println("QsysData:");
//    Serial.println(QsysData);

        // send a reply to the IP address and port that sent us the packet we received
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        while(QsysData != NULL)
    {
//        Serial.print(PinNum-1);
//        Serial.print(" ");
//        Serial.println(QsysData);
//        Serial.println(" ");
        


        //----------------------------------Handle GPIO Pin States---------------------------------------------------
        if(strcmp(QsysData, "Unused") == 0) {
          Udp.write("Unused:");
//          Serial.println("- : ");
        }
        
        // Set GPIO pin to analog
        if(strcmp(QsysData, "Log") == 0 || strcmp(QsysData, "A/D Input") == 0) {
          pinMode(PinNum+4, INPUT);    // sets the analog pin as input
          Udp.print(analogRead(PinNum-10));
          Udp.write(":");
//          Serial.print("inA");
//          Serial.print(PinNum-10);
//          Serial.println(" : ");
        }
        
        // Set GPIO pin to digital input
        if(strcmp(QsysData, "Digital") == 0) {
          if (PinNum < 10) {
            pinMode(PinNum, INPUT);      // sets the digital pin as input
            Udp.print(digitalRead(PinNum));
            Udp.write(":");
//            Serial.print("inD");
//            Serial.print(PinNum);
//            Serial.println(" : ");
          }
          else {
            pinMode(PinNum+4, INPUT);    // sets the analog pin as input
            Udp.print(digitalRead(PinNum+4));
            Udp.write(":");
//            Serial.print("inD");
//            Serial.print(PinNum+4);
//            Serial.println(" : ");
          }
        }



        //----------------------------------Handle GPIO OUTPUTS---------------------------------------------------
        // if packets are "true" turn on GPIO pin
        if(strcmp(QsysData, "true") == 0) {
          if (PinNum < 10) {
            pinMode(PinNum, OUTPUT);      // sets the digital pin as output
            digitalWrite(PinNum, HIGH);
            Udp.write("Dout:");
//            Serial.print("outD");
//            Serial.print(PinNum);
//            Serial.println(" ON : ");
          }
          else {
            pinMode(PinNum+4, OUTPUT);    // sets the analog pin as output
            digitalWrite(PinNum+4, HIGH);
            Udp.write("Dout:");
//            Serial.print("outD");
//            Serial.print(PinNum+4);
//            Serial.println(" ON : ");
          }
        }

        // if packets are "false" turn off GPIO pin
        if(strcmp(QsysData, "false") == 0) {
          if (PinNum < 10) {
            digitalWrite(PinNum, LOW);
            Udp.write("Dout:");
//            Serial.print("outD");
//            Serial.print(PinNum);
//            Serial.println(" OFF : ");
          }
          else {
            digitalWrite(PinNum+4, LOW);
            Udp.write("Dout:");
//            Serial.print("outD");
//            Serial.print(PinNum+4);
//            Serial.println(" OFF : ");
          }
        }
        

        //----------------------------------Handle ID Light---------------------------------------------------
        // if packets are ID request, respond and enable or disable ID light
        if(strcmp(QsysData, "IDON") == 0) {
          digitalWrite(IDpin, HIGH);
//          Serial.println("ID ON");
//          Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//          Udp.write("Hello Q-SYS");
//          Udp.endPacket();
        }
    
        if(strcmp(QsysData, "IDOFF") == 0) {
          digitalWrite(IDpin, LOW);
//          Serial.println("ID OFF");
//          Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//          Udp.write("Bye Q-SYS");
//          Udp.endPacket();
        }

        
        PinNum++;
        QsysData = strtok(NULL, ":"); 
    }

    
    Udp.print(millis());
    Udp.write(":");
    Udp.print(vccValue);  //Arduino refence voltage
    Udp.write(":");
    Udp.endPacket();
    PinNum = 2;

  } 
//========== End of UDP response =============================== 
  
  
  
  
//========== Handle Web UI ===============================  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean currentLineIsGet = true;
    int tCount = 0;
    char tBuf[64];
    char *pch;



    //Serial.print("Client request: ");

    while (client.connected()) {
      while(client.available()) {
        char c = client.read();

        if(currentLineIsGet && tCount < 63)
        {
          tBuf[tCount] = c;
          tCount++;
          tBuf[tCount] = 0;          
        }

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response
          while(client.available()) client.read();
          //Serial.println(tBuf);
          pch = strtok(tBuf,"?");

          while(pch != NULL)
          {
            if(strncmp(pch,"H=",2) == 0)
            {
              N = pch+2;
              N.toCharArray(HostName, 15);
//              Serial.println(HostName);
              EEPROM.put(100, HostName);   // save value to the eeprom      
//              Serial.print("H=");              
//              Serial.println(H);
            }
            
            if(strncmp(pch,"Mode=",5) == 0)
            {
              N = pch+5;
              N.toCharArray(StaticMode, 7);
              EEPROM.put(50, StaticMode);   // save value to the eeprom
//              Serial.print("S=");
//              Serial.println(S);             
            }
            
            if(strncmp(pch,"A=",2) == 0)
            {
              ipA = atoi(pch+2);
              EEPROM.put(10, ipA);   // save value to the eeprom
//              Serial.print("A=");
//              Serial.println(A,DEC);             
            }

            if(strncmp(pch,"B=",2) == 0)
            {
              ipB = atoi(pch+2);
              EEPROM.put(20, ipB);   // save value to the eeprom
//              Serial.print("B=");
//              Serial.println(B,DEC);             
            }

            if(strncmp(pch,"C=",2) == 0)
            {
              ipC = atoi(pch+2);
              EEPROM.put(30, ipC);   // save value to the eeprom
//              Serial.print("C=");
//              Serial.println(C,DEC);             
            }
            
            if(strncmp(pch,"D=",2) == 0)
            {
              ipD = atoi(pch+2);
              EEPROM.put(40, ipD);   // save value to the eeprom      
//              Serial.print("D=");              
//              Serial.println(D,DEC);
            }

            if(strncmp(pch,"P=",2) == 0)
            {
              Port = atoi(pch+2);
              EEPROM.put(60, Port);   // save value to the eeprom      
//              Serial.print("D=");              
//              Serial.println(D,DEC);
              delay(20);
              resetFunc();  //call reset
            }



            pch = strtok(NULL,"& ");
          }
//          Serial.println("Sending response");
          client.write("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><H1>REMOTE GPIO NANO</H1>");

          client.write("This is a GPIO network peripheral designed for the Q-SYS architecture.");
          client.write(brk);
 
          client.write("<form method=GET>Hostname: <input type=text name=H maxlength=20 value=");
          client.print(HostName);
          client.write(">");
          client.write(brk);
          client.write("<select name=Mode >");
          client.write("<option value=Auto");
          if (sel == true) {
            client.write(" selected");
          }
          client.write(">Auto</option>");
          client.write("<option value=Static");
          if (sel == false) {
            client.write(" selected");
          }
          client.write(">Static</option>");
          client.write("</select> ");


          client.write("IP Address:");
          client.write(numBox);
          client.write("A");
          client.write(ipRange);
          client.print(ipAget);
          client.write(">.");
          client.write(numBox);
          client.write("B");
          client.write(ipRange);
          client.print(ipBget);
          client.write(">.");
          client.write(numBox);
          client.write("C");
          client.write(ipRange);
          client.print(ipCget);
          client.write(">.");
          client.write(numBox);
          client.write("D");
          client.write(ipRange);
          client.print(ipDget);
          client.write(">");
          client.print(brk);
          

          client.write("UDP port:");
          client.write(numBox);
          client.write("P min=1 max=65535 value=");
          client.print(Port);
          client.write(">");
          client.write(brk);
          client.write("<input type=submit></form>");

          client.write("Designed by: Victor R Ellis");
          
          client.write("</body></html>\r\n\r\n");
          client.stop();
        }
        else if (c == '\n') {
          currentLineIsBlank = true;
          currentLineIsGet = false;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
//    Serial.println("done");
  }
//========== End of Web UI ===============================  


  delay(1);
}
