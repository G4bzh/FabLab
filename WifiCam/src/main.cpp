/* Need Base64 library by Arturo Guadalupi */

#include <Arduino.h>
#include <Base64.h>
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
    //atESP_setCWJAP(&ESPSerial, "Backoffice", "back4cyim");
    atESP_setCWJAP(&ESPSerial, "NETGEAR", "fablab35320");
    atESP_getCIFSR(&ESPSerial, ip);
    Serial.print("Got IP: ");
    Serial.println(ip);
    atESP_setCIPMODE(&ESPSerial,atESP_NORMAL);
    if ( atESP_setCIPSTART(&ESPSerial,"TCP","192.168.0.40",5000) == EXIT_SUCCESS )
    {
      const char* msg = "\x00\x00\x01\x00\x01\x00\x01\x01\x00\x00\x01\x00\x18\x00\x30\x00\x00\x00\x16\x00\x00\x00\x28\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x01\x00\x18\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\x00\x00";
      //const char* msg = "\x00\x00\x01\x00\x01\x00\x10\x10\x00\x00\x01\x00\x20\x00\x28\x04\x00\x00\x16\x00\x00\x00\x28\x00\x00\x00\x10\x00\x00\x00\x20\x00\x00\x00\x01\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xae\xc5\xfe\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
      atESP_sendData(&ESPSerial, msg, 70, "new.ico");
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
