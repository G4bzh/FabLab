/*
 * Includes
 *
 */

#include <stdlib.h>
#include "TimerOne.h"
#include "BT.h"
#include "Motor.h"
#include "Ultrasound.h"

/*
 * Constants
 *
 */
 
#define OUT_BUZZER  A3
 
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
bool buzzflag = false;


/*
 * Buzzer Interrupt
 *
 */

void buzzInterrupt()
{
  static int freq = 500;
  static char step = 1;

  if (!buzzflag)
  {
	  noTone(OUT_BUZZER);
	  return;
  }
  
  freq += step;
  
  if (freq > 1000)
  {
    step = -1;
  }

  if (freq < 500)
  {
    step = 1;
  }

  tone(OUT_BUZZER, freq);

  return;
} 
 
 
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
 * BT Josystick button 1 handler
 *
 */
 
void bt_button1_handler(uint8_t state)
{
	if (state == BT_BUTTON_UP)
	{
		buzzflag = false;
	}
	else
	{
		buzzflag = true;
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
	
	if  (dist < 30)
	{
	    unsigned long cur_dist, max_dist;
		uint8_t N,i,best_measure;
		
		/* Turn around to check obstacles */
		dir = MOTOR_BACKWARD;
		N = 100;

		/* Init Position */
		motor_setSpeed(leftMotor,0);
		motor_setSpeed(rightMotor,150);

		for(i=0; i<N; i++)
		  {
		    /* Tempo */
		    ultrasound_distance(ultra);
		    motor_run(leftMotor, dir);
		    motor_run(rightMotor, dir);
		  }
		

		max_dist = ultrasound_distance(ultra);
		cur_dist = 0;
		best_measure = 0;
		motor_setSpeed(leftMotor,150);
		motor_setSpeed(rightMotor,0);

		/* We take 2N measures and keep best one */
		for(i=0; i<2*N; i++)
		  {
		    motor_run(leftMotor, dir);
		    motor_run(rightMotor, dir);

		    cur_dist = ultrasound_distance(ultra);
		    if ( cur_dist > max_dist )
		      {
			max_dist = cur_dist;
			best_measure = i;
		      }
		  }

		motor_setSpeed(leftMotor,0);
		motor_setSpeed(rightMotor,150);
		
		/* Back to the best measure */
		for(i=0; i<(2*N-best_measure); i++)
		  {
		    /* Tempo */
		    ultrasound_distance(ultra);
		    motor_run(leftMotor, dir);
		    motor_run(rightMotor, dir);
		  }
		
	}
	
	/* Let's go */
	dir = MOTOR_FORWARD;
	leftSpeed = 222;
	rightSpeed = 222;
	
	return;
}



/*
 * Arduino Init
 *
 */

void setup() 
{
	
	
	pinMode(OUT_BUZZER,OUTPUT);
	Timer1.initialize(3000);
	Timer1.attachInterrupt(buzzInterrupt);
	
	
	Serial.begin(9600);

	bt_joystick = bt_create(&BT_serial, 9600);
	bt_setButtonHandler(bt_joystick,0,bt_button0_handler);
	bt_setButtonHandler(bt_joystick,1,bt_button1_handler);
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
	motor_setSpeed(rightMotor,rightSpeed);
	motor_run(leftMotor, dir);
	motor_run(rightMotor, dir);
	
	sprintf(str_dist,"%d",dist);
	bt_send(bt_joystick,(char*)"Obst.",str_dist,(char*)"cm");
		
	return;
}
