// Logiciel de configuration du modle HC05
// 
// Pour configurer le module HC05n il faut le mettre en mode AT en appuyant sur le bouton situé sur celui-ci au moment de la mise sous tension
// -> on peut vérifier que le module est bien en mode AT car il clignote lentement.
// Ensuite, il est possible de vérifier, via le terminal arduino, que tout fonctionne en rentrant la commande AT
// -> le module doit renvoiyer OK
//
//  
// Lecture du nom : AT+NAME?  (à envoyer en pressant le bouton sur le module, sinon, la commande est ignorée
// Changement du nom : AT+NAME=NOUVEAUNOM  (à envoyer en pressant le bouton sur le module, sinon, la commande est ignorée
// Lecture du baudrate : AT+UART
// Changement du baudrate : AT+UART=57600,0,0
// Lecture du mot de passe : AT+PSWD?  (à envoyer en pressant le bouton sur le module, sinon, la commande est ignorée
// Changement du mot de passe : AT+PSWD=NOUVEAUMOTDEPASSE  (à envoyer en pressant le bouton sur le module, sinon, la commande est ignorée
// Liste des commandes : http://www.martyncurrey.com/wp-content/uploads/2014/12/AT-commands-list.jpg

 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(3, 4); // RX | TX
 
char c = ' ';
 
void setup() 
{
    Serial.begin(9600);

    delay(1000);
    
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    BTserial.begin(38400);  

}
 
void loop()
{
 
    // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        c = BTserial.read();
        Serial.write(c);
    }
 
    // Keep reading from Arduino Serial Monitor and send to HC-05
    if (Serial.available())
    {
        c =  Serial.read();
        BTserial.write(c);  
    }
 
}


