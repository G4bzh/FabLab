#include <Arduino.h>
#include <SoftwareSerial.h>
#include <atESP.h>

#define WAIT 200

SoftwareSerial ESPSerial(3, 4); // RX | TX



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
    Serial.println(ip);

}

char c;

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
