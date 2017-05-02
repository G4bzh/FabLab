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

bool autoflag = false;
uint8_t avoid = 0;

/*
 * BT Josystick button 0 handler
 *
 */
 
void bt_button0_handler(uint8_t state)
{
	if (state == BT_BUTTON_UP)
	{
		autoflag = false;
	}
	else
	{
		autoflag = true;
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
	
	/* No handling in autopilot mode */
	if (autoflag)
	{
		return;
	}
	
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
 * Autopilot
 *
 */

void autopilot(unsigned long dist)
{
	/* Be sure to be in autopilot mode */
	if (!autoflag)
	{
		return;
	}
	
	if ( (dist > 30) && (!avoid) )
	{
		dir = MOTOR_FORWARD;
		leftSpeed = 222;
		rightSpeed = 222;
	}
	else
	{
		unsigned long d;
		
		/* We are avoiding an obstacle */
		avoid++;
		

		
		/* Turn around to check obstacles */
		dir = MOTOR_BACKWARD;
		leftSpeed = 0;
		rightSpeed = 123;
		
		
		/* We take 2N measures 
		
		// Go to initial position for measuring
		for i=0 to N
			ultrasound_distance(ultra); // tempo
			turnLeft();
		
		// Find best angle
		max_dist = ultrasound_distance(ultra);
		cur_dist = 0;
		best_measure = 0;
		
		for i=0 to 2N
			turnRight();
			cur_dist = ultrasound_distance(ultra);
			
			if ( cur_dist > max_dist )
				max_dist = cur_dist;
				best_measure = i;
		
		// Back to best measure
		for i=0 to (2N-best_measure)
			ultrasound_distance(ultra); // tempo
			turnLeft();
			
		// Go
		
		*/
		
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


	char str_dist[10];
	unsigned long dist;
	
	dist = ultrasound_distance(ultra);
	
	bt_run(bt_joystick);
	autopilot(dist);
	
	motor_setSpeed(leftMotor,leftSpeed);
	motor_run(leftMotor, dir);
	motor_setSpeed(rightMotor,rightSpeed);
	motor_run(rightMotor, dir);
	
	sprintf(str_dist,"%d",dist);
	bt_send(bt_joystick,"Obst.",str_dist,"cm");
		
	return;
}