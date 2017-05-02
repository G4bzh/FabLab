/*
 * 
 * Includes
 *
 */

#include <stdint.h> 
#include <stdlib.h>
#include <Arduino.h>
#include "Ultrasound.h"


/* 
 * Constructor 
 *
 */
 
 
ULTRASOUND* ultrasound_create(uint8_t trigger, uint8_t echo)
{
	ULTRASOUND* u;
	
	u = (ULTRASOUND*)malloc(sizeof(ULTRASOUND));
	if ( u == NULL)
	{
		return NULL;
	}
	
	u->trigger = trigger;
	u->echo = echo;
	
	pinMode(trigger, OUTPUT);
	digitalWrite(trigger, LOW);
	pinMode(echo, INPUT);
	
	return u;
}

/* 
 * Destructor 
 *
 */
 
 
void utlrasound_delete(ULTRASOUND* u)
{
	if (u != NULL)
	{
		free(u);
	}
	
	return;
}


/* 
 * Distance
 *
 */
 
unsigned long ultrasound_distance(ULTRASOUND* u)
{
	if (u == NULL)
	{
		return 0;
	}
	
    /* Generate a 10us pulse */
    digitalWrite(u->trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(u->trigger, LOW);
    
    /* Read echo pulse timing */
    return (pulseIn(u->echo, HIGH)/58);

}