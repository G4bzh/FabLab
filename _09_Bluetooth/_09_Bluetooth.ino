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

#define   RX    3 
#define   TX    4

#define   STX   0x02
#define   ETX   0x03

byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};

SoftwareSerial BTSERIAL(RX, TX); 



void BT_Button(int flag)
{
  switch (flag)
  {
    case 'A':
    {
       /* B1 ON */
       Serial.println("B1 ON");
       break;
    }

    case 'B':
    {
       /* B1 OFF */
       Serial.println("B1 OFF");
       break;
    }

    case 'C':
    {
       /* B2 ON */
       Serial.println("B2 ON");
       break;
    }

    case 'D':
    {
       /* B2 OFF */
       Serial.println("B2 OFF");
       break;
    }

    case 'E':
    {
       /* B3 ON */
       Serial.println("B3 ON");
       break;
    }

    case 'F':
    {
       /* B3 OFF */
       Serial.println("B3 OFF");
       break;
    }

    case 'G':
    {
       /* B4 ON */
       Serial.println("B4 ON");
       break;
    }

    case 'H':
    {
       /* B4 OFF */
       Serial.println("B4 OFF");
       break;
    }

    case 'I':
    {
       /* B5 ON */
       Serial.println("B5 ON");
       break;
    }

    case 'J':
    {
       /* B5 OFF */
       Serial.println("B5 OFF");
       break;
    }

    case 'K':
    {
       /* B6 ON */
       Serial.println("B6 ON");
       break;
    }

    case 'L':
    {
       /* B6 OFF */
       Serial.println("B6 OFF");
       break;
    }
 
  }

  return;
  
}


void BT_Josytick(byte cmd[8])
{
  int X = (cmd[1]-48)*100 + (cmd[2]-48)*10 + (cmd[3]-48);       // obtain the Int from the ASCII representation
  int Y = (cmd[4]-48)*100 + (cmd[5]-48)*10 + (cmd[6]-48);
  X = X - 200;                                                  // Offset to avoid
  Y = Y - 200;                                                  // transmitting negative numbers

  if(X<-100 || X>100 || Y<-100 || Y>100)     return;      // commmunication error
 
  Serial.print("Joystick:  ");
  Serial.print(X);  
  Serial.print(", ");  
  Serial.println(Y); 
  
  return;
}


void setup() {
  // put your setup code here, to run once:

  BTSERIAL.begin(9600);
  Serial.begin(9600);
  Serial.println("Go");

}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  
  if( BTSERIAL.available() )  
  { 

                     
     delay(2);
     cmd[0] =  BTSERIAL.read();  
     
     if(cmd[0] == STX)  
     {
       i=1;    
       
       while(BTSERIAL.available())  
       {
       
         delay(1);
         cmd[i] = BTSERIAL.read();
         
         if ( (cmd[i] > 127) || (i > 7) )
         {
            break;     // Communication error
         }
  
         
         if ( (cmd[i]==ETX) && ( (i==2) || (i==7) ) )
         {
            break;     // Button or Joystick data
         }
         
         i++;
       }
  
       
       if (i==2)
       {
          BT_Button(cmd[1]);    // 3 Bytes  ex: < STX "C" ETX >
       }
       
       if(i==7)
       {
          BT_Josytick(cmd);     // 6 Bytes  ex: < STX "200" "180" ETX >
       }
       
     }
     
   } 
   
}
