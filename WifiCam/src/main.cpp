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
    Serial.println("Remember to select Both NL & CR in the serial monitor :)");

    ESPSerial.begin(9600);



}

void loop()
{

  Serial.println("Setting AP mode (1/2)");
  Serial.println(atESP_setCWMODE(&ESPSerial, atESP_STA));
  Serial.println(atESP_getCWMODE(&ESPSerial));

  Serial.println("Setting AP mode (2/2)");
  Serial.println(atESP_setCWMODE(&ESPSerial, atESP_STAP));
  Serial.println(atESP_getCWMODE(&ESPSerial));

}
