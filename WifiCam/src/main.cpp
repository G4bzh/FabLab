#include <Arduino.h>
#include <SoftwareSerial.h>
#include <atESP.h>

#define WAIT 200

SoftwareSerial ESPSerial(3, 4); // RX | TX
char c;


void setup()
{
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Arduino is ready");

    ESPSerial.begin(9600);
    while (!ESPSerial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("ESP is ready");

    char ip[16];
    atESP_setCWJAP(&ESPSerial, "Backoffice", "back4cyim");
    atESP_getCIFSR(&ESPSerial, ip);
    Serial.print("Got IP: ");
    Serial.println(ip);
    atESP_setCIPMODE(&ESPSerial,atESP_NORMAL);
    if ( atESP_setCIPSTART(&ESPSerial,"TCP","192.168.218.147",8080) == EXIT_SUCCESS )
    {
      Serial.println("Successfully connected to server");
      atESP_setCIPSEND(&ESPSerial,"HELLO");
      atESP_setCIPCLOSE(&ESPSerial);
    }
    else
    {
      Serial.println("Unable to connect to server");
    }
}

void loop()
{

  if (ESPSerial.available())
  {
     c = ESPSerial.read();
     Serial.write(c);
  }
  if (Serial.available())
  {
     c =  Serial.read();
     ESPSerial.write(c);
  }

}
