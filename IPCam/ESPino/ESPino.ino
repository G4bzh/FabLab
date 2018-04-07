/*
 * Issue AT+IPR=9600
 */


#include <SoftwareSerial.h>

#define WAIT 100

SoftwareSerial ESPserial(3, 4); // RX | TX


void sendESP(SoftwareSerial* ss, char* cmd, char* buff, int* sz)
{
  int i,n,k,ok;
  char c;

  k=0;
  ok=0;

  /* Send command */
  ss->println(cmd);

  
  while(!ok)
  {
  
    if (n=ss->available())
    {

      /* Fill buffer with new data */
      for(i=0;i<n;i++)
      {
        buff[k+i] = ss->read();
      }
      /* Update counter */
      k+=i;

      /* Look for "OK\r" that terminates an AT command response */
      for(i=0;i<k-2;i++)
      {
        if ( (buff[i] == 'O') && (buff[i+1] == 'K') && (buff[i+2] == '\r') )
        {
          /* Found ! */
          ok = 1;
        }
      }
      
    }
    
  }

  /* Set buffer size */
  *sz = k;
  /* Null terminate buffer */
  buff[k] = 0;
  
  return;
}

void setup() 
{
    Serial.begin(9600);

    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    ESPserial.begin(9600);  
      


}
 
void loop()
{
  char str[1024];
  int n;
  
  sendESP(&ESPserial,"AT+CWLAP", str, &n);
  delay(WAIT);
  Serial.print("Got ");
  Serial.print(n),
  Serial.println(" bytes :");
  Serial.println(str);
  
}
