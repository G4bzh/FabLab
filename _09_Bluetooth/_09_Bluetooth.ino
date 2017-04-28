#include <SoftwareSerial.h>                                

/*
 * RX(arduino) -> TX (BT)
 * TX(arduino) -> RX (BT)
 * 
 * No need voltage divisor if HC05 for RX (BT)
 * To configure: go in AT Mode by powering off BT, press button and hold during power on
 * then serial communication can start a 38400bauds with both NL CR
 * 
 * We can then exchange with serial monitor with this code  :
 * 
 *   
 *   
 
          if (BTSERIAL.available())                  
          {
            
            Serial.write(BTSERIAL.read());
        
          }
         
          if (Serial.available())
          {
            
            BTSERIAL.write(Serial.read());
        
          }
  
 * 
 */

#define RX  3 
#define TX  4

SoftwareSerial BTSERIAL(RX, TX);  

void setup() {
  // put your setup code here, to run once:

  BTSERIAL.begin(38400);
  Serial.begin(9600);

  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (BTSERIAL.available())                  
  {
    
    Serial.write(BTSERIAL.read());

  }
 
  if (Serial.available())
  {
    
    BTSERIAL.write(Serial.read());

  } 
}
