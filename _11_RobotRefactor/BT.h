#ifndef _BT_H_
#define _BT_H_

#include <stdint.h>
#include <SoftwareSerial.h> 

#define BT_CMD_LEN   8
#define BT_BUTTONS_LEN 6

#define BT_BUTTON_UP  0
#define BT_BUTTON_DOWN  1

/* Button & Padd handler type */
typedef void (*BT_b_handler)(uint8_t state);
typedef void (*BT_p_handler)(int X, int Y);

typedef struct 
{
	SoftwareSerial* sserial;
	int bauds;
	uint8_t cmd[BT_CMD_LEN];
	char buttons[BT_BUTTONS_LEN+1];
	BT_b_handler b_handlers[BT_BUTTONS_LEN];
	BT_p_handler p_handler;
} BT_JOYSTICK;


BT_JOYSTICK* bt_create(SoftwareSerial* sserial, int bauds);
void bt_delete(BT_JOYSTICK* j);
void bt_setButtonHandler(BT_JOYSTICK* j, uint8_t b, BT_b_handler f);
void bt_setPaddleHandler(BT_JOYSTICK* j, BT_p_handler f);
void bt_run(BT_JOYSTICK* j);
void bt_send(BT_JOYSTICK* j, char* d1, char* d2, char* d3);

#endif

