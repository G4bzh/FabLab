
#define SORTIE_BUZZER   A3        // broche sur laquelle le haut-parleur est branché

void setup()   { 
  pinMode(SORTIE_BUZZER, OUTPUT); // configure la broche en sortie 
} 

void loop(){  
  int i;

  for (i=500; i<1000; i++) {    // on envoie une fréquence de 500 à 1000Hz
    tone(SORTIE_BUZZER, i); 
    delay(1);                   // attente de 1ms
  } 
  
  for (i=1000; i>500; i--) {    // on envoie une fréquence de 1000 à 500Hz
    tone(SORTIE_BUZZER, i); 
    delay(1);                   // attente de 1ms
  } 
} 




