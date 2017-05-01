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


/*
 * Includes
 *
 */

#include <stdint.h> 
#include <stdlib.h>
#include <Arduino.h>
#include <SoftwareSerial.h> 
#include "BT.h"

/*
 * Private Prototype
 *
 */
  
void bt_paddle(BT_JOYSTICK* j);
void bt_button(BT_JOYSTICK* j);

  
/*
 * Constants
 *
 */
 
#define   STX   0x02
#define   ETX   0x03

 
/* 
 * Constructor 
 *
 */
 
BT_JOYSTICK* bt_create(SoftwareSerial* sserial, int bauds)
{
	BT_JOYSTICK* j;
	uint8_t i;
	
	if (sserial == NULL)
	{
		return NULL;
	}
	
	j = (BT_JOYSTICK*)malloc(sizeof(BT_JOYSTICK));
	if (j == NULL)
	{
		return NULL;
	}
	
	j->bauds = bauds;
	j->sserial = sserial;
	j->sserial->begin(bauds);

	
	for(i=0; i<BT_BUTTONS_LEN; i++)
	{
		j->buttons[i] = '0';
		j->b_handlers[i] = NULL;
	}
	j->buttons[i] = '\0';
	
	for(i=0; i<BT_CMD_LEN; i++)
	{
		j->cmd[i] = 0;
	}
	
	j->p_handler = NULL;
	
	
	
	return j;
}



/* 
 * Destructor 
 *
 */

void bt_delete(BT_JOYSTICK* j)
{
	if (j != NULL)
	{
		free(j);
	}
	
	return;
}


/*
 * Set handlers
 *
 */
 
void bt_setButtonHandler(BT_JOYSTICK* j, uint8_t b, BT_b_handler f)
{
	if ( (j == NULL) || (b >= BT_BUTTONS_LEN) || (f == NULL) )
	{
		return;
	}

	j->b_handlers[b] = f;
	
	return;
} 
 
void bt_setPaddleHandler(BT_JOYSTICK* j, BT_p_handler f)
{
	if ( (j == NULL) || (f == NULL) )
	{
		return;
	}

	j->p_handler = f;
	
	return;
} 


/*
 * Run
 *
 */

void bt_run(BT_JOYSTICK* j)
{
	uint8_t i;

	if ( j == NULL )
	{
		return;
	}


	if( j->sserial->available() )  
	{ 	 
		delay(2);
		j->cmd[0] =  j->sserial->read();  

		if(j->cmd[0] == STX)  
		{
			i=1;    

			while(j->sserial->available())  
			{

			 delay(1);
			 j->cmd[i] = j->sserial->read();
			 
			 if ( (j->cmd[i] > 127) || (i >= BT_CMD_LEN) )
			 {
				break;     // Communication error
			 }

			 
			 if ( (j->cmd[i]==ETX) && ( (i==2) || (i==BT_CMD_LEN-1) ) )
			 {
				break;     // Button or Joystick data
			 }
			 
			 i++;
		}


		if (i==2)
		{
			bt_button(j);    // 3 Bytes  ex: < STX "C" ETX >
		}

		if(i==7)
		{
			bt_paddle(j);     // 6 Bytes  ex: < STX "200" "180" ETX >
		}

		}
	}

	return;  
}

/*
 * Paddle & Button trampoline
 *
 */

void bt_paddle(BT_JOYSTICK* j)
{
	int X,Y;
	
	/* Obtain the Int from the ASCII representation */
	X = (j->cmd[1]-48)*100 + (j->cmd[2]-48)*10 + (j->cmd[3]-48);
	Y = (j->cmd[4]-48)*100 + (j->cmd[5]-48)*10 + (j->cmd[6]-48);
	
	/* Offset to avoid transmitting negative numbers */
	X = X - 200;
	Y = Y - 200;

	/* Commmunication error ? */
	if ( X<-100 || X>100 || Y<-100 || Y>100 )
	{
		return;
	}
  
	if (j->p_handler != NULL)
	{
		j->p_handler(X,Y);
	}
  
	return;
}

void bt_button(BT_JOYSTICK* j)
{

	if ( j == NULL )
	{
		return;
	}
	
	switch (j->cmd[1])
	{
		case 'A':
		{
		   /* B1 ON */
		   j->buttons[5] = '1'; 
		   if (j->b_handlers[0] != NULL)
		   {
			   j->b_handlers[0](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'B':
		{
		   /* B1 OFF */
		   j->buttons[5] = '0'; 
		   if (j->b_handlers[0] != NULL)
		   {
			   j->b_handlers[0](BT_BUTTON_UP);
		   }
		   break;
		}

		case 'C':
		{
		   /* B2 ON */
		   j->buttons[4] = '1';  
		   if (j->b_handlers[1] != NULL)
		   {
			   j->b_handlers[1](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'D':
		{
		   /* B2 OFF */
		   j->buttons[4] = '0'; 
		   if (j->b_handlers[1] != NULL)
		   {
			   j->b_handlers[1](BT_BUTTON_UP);
		   }
		   break;
		}

		case 'E':
		{
		   /* B3 ON */
		   j->buttons[3] = '1'; 
		   if (j->b_handlers[2] != NULL)
		   {
			   j->b_handlers[2](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'F':
		{
		   /* B3 OFF */
		   j->buttons[3] = '0'; 
		   if (j->b_handlers[2] != NULL)
		   {
			   j->b_handlers[2](BT_BUTTON_UP);
		   }
		   break;
		}

		case 'G':
		{
		   /* B4 ON */
		   j->buttons[2] = '1'; 
		   if (j->b_handlers[3] != NULL)
		   {
			   j->b_handlers[3](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'H':
		{
		   /* B4 OFF */
		   j->buttons[2] = '0'; 
		   if (j->b_handlers[3] != NULL)
		   {
			   j->b_handlers[3](BT_BUTTON_UP);
		   }
		   break;
		}

		case 'I':
		{
		   /* B5 ON */
		   j->buttons[1] = '1'; 
		   if (j->b_handlers[4] != NULL)
		   {
			   j->b_handlers[4](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'J':
		{
		   /* B5 OFF */
		   j->buttons[1] = '0'; 
		   if (j->b_handlers[4] != NULL)
		   {
			   j->b_handlers[4](BT_BUTTON_UP);
		   }
		   break;
		}

		case 'K':
		{
		   /* B6 ON */
		   j->buttons[0] = '1';  
		   if (j->b_handlers[5] != NULL)
		   {
			   j->b_handlers[5](BT_BUTTON_DOWN);
		   }
		   break;
		}

		case 'L':
		{
		   /* B6 OFF */
		   j->buttons[0] = '0';  
		   if (j->b_handlers[5] != NULL)
		   {
			   j->b_handlers[5](BT_BUTTON_UP);
		   }
		   break;
		}

	}

  
	return;
}


/*
 * Send back to joystick
 *
 */
 
 
void bt_send(BT_JOYSTICK* j, char* d1, char* d2, char* d3)
{
	static long previousMillis = 0;                             
	long currentMillis = millis();

	if (j == NULL)
	{
		return;
	}
	
	/* Tempo at 250ms */
	if(currentMillis - previousMillis > 250) 
	{ 

		previousMillis = currentMillis; 

		j->sserial->print((char)STX);
		j->sserial->print(j->buttons);  
		j->sserial->print((char)0x1);  
		j->sserial->print(d1);          
		j->sserial->print((char)0x4);  
		j->sserial->print(d2); 
		j->sserial->print((char)0x5);  
		j->sserial->print(d3); 
		j->sserial->print((char)ETX);  
	}
	
	return;
	
}