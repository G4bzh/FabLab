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
  int n,i;
  char** ssid;

  ssid = (char**)malloc(20*sizeof(char*));
  for(i=0;i<20;i++)
  {
    ssid[i] = (char*)malloc(20*sizeof(char));
  }

  atESP_CWLAP(&ESPSerial, ssid, &n);

  Serial.print("Got ");
  Serial.print(n),
  Serial.println(" SSID :");
  for(i=0;i<n;i++)
  {
    Serial.println(ssid[i]);
  }

  for(i=0;i<20;i++)
  {
    free(ssid[i]);
  }
  free(ssid);

}
