/*
 * Includes
 *
 */

#include <stdint.h>
#include "BT.h"


/*
 * Global
 *
 */

int n=0;
 
SoftwareSerial BT_serial(3, 4); 
BT_JOYSTICK* bt_joystick;


/*
 * BT Josystick button 0 handler
 *
 */
 
void bt_button0_handler(uint8_t state)
{
	if (state == BT_BUTTON_UP)
	{
		Serial.println("Button 0 released");
		n++;
	}
	else
	{
		Serial.println("Button 0 pressed");
		n++;
	}

	return;
}

/*
 * BT Josystick Paddle handler
 *
 */
 
void bt_paddle_handler(int X, int Y)
{
	Serial.print("Joystick:  ");
	Serial.print(X);  
	Serial.print(", ");  
	Serial.println(Y); 

	return;
}



/*
 * Arduino Init
 *
 */

void setup() 
{
	Serial.begin(9600);

	bt_joystick = bt_create(&BT_serial, 9600);
	bt_setButtonHandler(bt_joystick,0,bt_button0_handler);
	bt_setPaddleHandler(bt_joystick,bt_paddle_handler);
	
	return;
}


/*
 * Arduino Loop
 *
 */
 
 void loop()
{

	char str[4];
	
	sprintf(str,"%d",n);
	
	bt_run(bt_joystick);
	bt_send(bt_joystick,"B1",str,"times");
	
	return;
}