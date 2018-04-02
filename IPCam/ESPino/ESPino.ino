#include <SoftwareSerial.h>

#define WAIT 100

SoftwareSerial ESPserial(3, 4); // RX | TX

char c = ' ';


void sendESP(SoftwareSerial *ss, char* str)
{
    int n = strlen(str);
    int i;
    
    for(i=0;i<n;i++)
    {
       ss->write(str[i]);
       delay(WAIT);
    }
    
    return;
}


int checkESP(SoftwareSerial *ss)
{
  int n,i,r;
  char* str;
  
  while ( ! (n=ss->available()) )
  {
  }

  str = (char*)malloc((n+1)*sizeof(char));
  if ( str == NULL)
  {
    return 0;
  }

  for(i=0;i<n;i++)
  {
    str[i] = ss->read();
    delay(WAIT);
  }  
  str[i]=0;
  
  r = (str[i-2] == '>')?1:0;
  free(str);

  return r;

}
 
void setup() 
{
    Serial.begin(115200);

    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    ESPserial.begin(115200);  
      


}
 
void loop()
{
  
  sendESP(&ESPserial,"print('Hello')\r");
  if (checkESP(&ESPserial))
  {
    Serial.println("Command successfully sent !");
  }
  else
  {
    Serial.println("Fail to send command !"); 
  }
  
}
