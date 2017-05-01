#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdint.h>

#define MOTOR_FORWARD	0
#define MOTOR_BACKWARD	1
#define MOTOR_BRAKE		2

typedef struct
{
	uint8_t EN;
	uint8_t IN1;
	uint8_t IN2;
	uint8_t speed;
} MOTOR;


MOTOR* motor_create(uint8_t en, uint8_t in1, uint8_t in2);
void motor_delete(MOTOR* m);
void motor_setSpeed(MOTOR* m, uint8_t s);
void motor_run(MOTOR* m, uint8_t dir);

#endif
