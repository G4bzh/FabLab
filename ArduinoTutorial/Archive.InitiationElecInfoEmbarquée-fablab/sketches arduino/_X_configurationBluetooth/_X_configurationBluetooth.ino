/*
 * Application de configuration du module bluetooth HC06
 */
#include "SoftwareSerial.h"


SoftwareSerial bluetoothSerial(11,12);                         // BlueTooth module: pin#11=TX pin#12=RX


char nom[10]   = "rob57600"; // nom du module bluetooth
char bauds     = '7';         // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600, 5=>19200, 6=>38400, 7=>57600, 8=>115200
char password[10] = "1234";   // Mot de passe pour s'apairer avec le module bluetooth

void setup()
{
  Serial.begin(115200);
  bluetoothSerial.begin(9600);
}

void loop()
{
  delay(1000);
  bluetoothSerial.print("AT"); delay(1000);

  bluetoothSerial.print("AT+NAME"); bluetoothSerial.print(nom); delay(2000);

  bluetoothSerial.print("AT+BAUD"); bluetoothSerial.print(bauds); delay(2000);

  bluetoothSerial.print("AT+PIN"); bluetoothSerial.print(password); delay(2000);    
  bluetoothSerial.print("AT"); delay(2000);

  Serial.print("Done !!"); delay(2000);

}
