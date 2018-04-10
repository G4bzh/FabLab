#ifndef _ATESP_H_
#define _ATESP_H_

#include <SoftwareSerial.h>

/* CWMODE constants */
#define atESP_STA   1
#define atESP_AP    2
#define atESP_STAP  3

/* Protoypes */
int atESP_getCWLAP(SoftwareSerial* ss, char* ssid[], int* n);
int atESP_getCWMODE(SoftwareSerial* ss);
int atESP_setCWMODE(SoftwareSerial* ss, int mode);

#endif
