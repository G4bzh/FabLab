/*
 * Includes
 *
 */

#include <stdlib.h>
#include "BT.h"
#include "Motor.h"
#include "Ultrasound.h"

/*
 * Global
 *
 */


SoftwareSerial BT_serial(3, 4); 
BT_JOYSTICK* bt_joystick;

MOTOR* leftMotor;
MOTOR* rightMotor;
int dir = 0;
int leftSpeed = 0;
int rightSpeed = 0;

ULTRASOUND* ultra;



/*
 * BT Josystick button 0 handler
 *
 */
 
void bt_button0_handler(uint8_t state)
{
	if (state == BT_BUTTON_UP)
	{
		Serial.println("Button 0 released");
	}
	else
	{
		Serial.println("Button 0 pressed");
	}

	return;
}

/*
 * BT Josystick Paddle handler
 *
 */
 
void bt_paddle_handler(int X, int Y)
{
	int power;
	
	power = (int)(sqrt(X*X + Y*Y));
	
	/* Trigonometry : X is cosine */
	if (X>0)
	{
		leftSpeed = power;
		rightSpeed = power-X;
	}
	else
	{
		rightSpeed = power;
		leftSpeed = X+power;
	}
	
	/* Map speed between 0 and 250 */
	leftSpeed = (int)(constrain(leftSpeed,0,100)*2.5);
	rightSpeed = (int)(constrain(rightSpeed,0,100)*2.5);
	
	/* Y tells direction */
	if (Y > 0)
	{
		dir = MOTOR_FORWARD;
	}
	else
	{
		dir = MOTOR_BACKWARD;
	}		
	
	/* No movements here */
	if ( (Y==0) && (X==0) )
	{
		dir = MOTOR_BRAKE;
	}

	
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
	
	rightMotor = motor_create(5,10,9);
	leftMotor = motor_create(6,8,7);
	
	ultra = ultrasound_create(13,12);
	
	return;
}


/*
 * Arduino Loop
 *
 */
 
 void loop()
{


	bt_run(bt_joystick);

	motor_setSpeed(leftMotor,leftSpeed);
	motor_run(leftMotor, dir);
	motor_setSpeed(rightMotor,rightSpeed);
	motor_run(rightMotor, dir);
	
	Serial.print("Obstacle: ");
	Serial.println(ultrasound_distance(ultra));
		
	return;
}