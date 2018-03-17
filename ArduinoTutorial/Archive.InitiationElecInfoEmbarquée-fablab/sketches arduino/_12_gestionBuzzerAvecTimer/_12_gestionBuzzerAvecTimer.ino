
#include "timerOne.h"

#define SORTIE_BUZZER   A3 

void timer1Interrupt() {            // fonction appelée périodiquement
  static int freq = 500;
  static char pas = 1;

  freq = freq + pas;
  if (freq > 1000) pas = -1;
  if (freq < 500)  pas = 1;    
  tone(SORTIE_BUZZER, freq); 
}

void setup()   { 
  pinMode(SORTIE_BUZZER, OUTPUT); //met la broche en sortie 
 
  // initialisation interruption Timer 1
  Timer1.initialize(1000);                // configure un timer de 1000us
  Timer1.attachInterrupt( timer1Interrupt );  // toutes les 1000us, la fonction myInterrupt sera appelée
} 

void loop(){  
 
} 



