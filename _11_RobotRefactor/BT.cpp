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
 
BT_JOYSTICK* bt_create(SoftwareSerial* serial, int bauds)
{
	BT_JOYSTICK* j;
	uint8_t i;
	
	if (serial = NULL)
	{
		return NULL;
	}
	
	j = (BT_JOYSTICK*)malloc(sizeof(BT_JOYSTICK));
	if (j == NULL)
	{
		return NULL;
	}
	
	j->bauds = bauds;
	j->serial->begin(bauds);
	
	for(i=0; i<BT_BUTTONS_LEN; i++)
	{
		j->buttons[i] = '0';
		j->b_handlers[i] = NULL;
	}
	
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
	
	if ( j== NULL )
	{
		return;
	}
	
	if( j->serial->available() )  
	{ 
				 
		delay(2);
		j->cmd[0] =  j->serial->read();  

		if(j->cmd[0] == STX)  
		{
			i=1;    

			while(j->serial->available())  
			{

			 delay(1);
			 j->cmd[i] = j->serial->read();
			 
			 if ( (j->cmd[i] > 127) || (i > 7) )
			 {
				break;     // Communication error
			 }

			 
			 if ( (j->cmd[i]==ETX) && ( (i==2) || (i==7) ) )
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

	return;
}
