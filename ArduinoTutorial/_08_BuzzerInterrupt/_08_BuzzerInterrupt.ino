#include "TimerOne.h"

#define OUT_BUZZER  A3

void buzzInterrupt()
{
  static int freq = 500;
  static char step = 1;

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

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_BUZZER,OUTPUT);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(buzzInterrupt);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  

}
