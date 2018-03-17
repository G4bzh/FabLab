#ifndef _ULTRASOUND_H_
#define _ULTRASOUND_H_

#include <stdint.h>

typedef struct
{
	uint8_t trigger;
	uint8_t echo;
} ULTRASOUND;


ULTRASOUND* ultrasound_create(uint8_t trigger, uint8_t echo);
void ultrasound_delete(ULTRASOUND* u);
unsigned long ultrasound_distance(ULTRASOUND* u);

#endif