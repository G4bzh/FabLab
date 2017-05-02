/*
 * Includes
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>
#include "Motor.h"

/*
 * Constructor
 *
 */
 
MOTOR* motor_create(uint8_t en, uint8_t in1, uint8_t in2)
{
	MOTOR* m;
	
	m = (MOTOR*)malloc(sizeof(MOTOR));
	if ( m == NULL )
	{
		return NULL;
	}
	
	m->EN = en;
	m->IN1 = in1;
	m->IN2 = in2;
	m->speed = 0;
	
	pinMode(en, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	
	return m;
	
}

/*
 * Destructor
 *
 */
 
void motor_delete(MOTOR* m)
{
	free(m);
	return;
}


/*
 * Set speed
 *
 */
 
void motor_setSpeed(MOTOR* m, uint8_t s)
{
	if (m != NULL)
	{
		m->speed = s;
	}
	
	return;
}


/*
 * Run
 *
 */
 
void motor_run(MOTOR* m, uint8_t dir)
{
	if ( m == NULL )
	{
		return;
	}
	
	switch(dir)
	{
		case MOTOR_FORWARD:
		{
			digitalWrite(m->IN1, HIGH); 
			digitalWrite(m->IN2, LOW);
			analogWrite(m->EN, m->speed);
			break;
		}
		
		case MOTOR_BACKWARD:
		{
			digitalWrite(m->IN2, HIGH); 
			digitalWrite(m->IN1, LOW);
			analogWrite(m->EN, m->speed);
			break;
		}
		
		case MOTOR_BRAKE:
		{
			digitalWrite(m->IN1, HIGH); 
			digitalWrite(m->IN2, LOW);
			analogWrite(m->EN, 0);
			break;
		}
	}
}