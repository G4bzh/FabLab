#define MOTOR1_EN     11
#define MOTOR1_IN1    10
#define MOTOR1_IN2     9
#define MOTOR2_IN1     8
#define MOTOR2_IN2     7
#define MOTOR2_EN      6

#define ENTREE_BOUTON  2
#define SORTIE_LED    13

boolean arretUrgenceEnclenche = false;

void setup() {
  pinMode(ENTREE_BOUTON, INPUT_PULLUP);   //met la broche en entrée en activant le pullup interne 
  pinMode(SORTIE_LED,    OUTPUT);        //met la broche en sortie

  // configure les sorties utilisées pour piloter les deux moteurs
/*  pinMode(MOTOR1_EN,      OUTPUT); 
  pinMode(MOTOR1_IN1,     OUTPUT);      
  pinMode(MOTOR1_IN2,     OUTPUT);      
  pinMode(MOTOR2_EN,      OUTPUT); 
  pinMode(MOTOR2_IN1,     OUTPUT);      
  pinMode(MOTOR2_IN2,     OUTPUT);      
 */
}

void loop() {
  bool etatEntree = digitalRead(ENTREE_BOUTON);
    
  if (etatEntree == LOW) {
    arretUrgenceEnclenche = true;
  }

  if (!arretUrgenceEnclenche) {
    digitalWrite(MOTOR1_IN1,  HIGH);       // moteur 1 dans un sens
    digitalWrite(MOTOR1_IN2,  LOW);
    digitalWrite(MOTOR1_EN,   HIGH);
    digitalWrite(MOTOR2_IN1,  HIGH);       // moteur 2 dans un sens
    digitalWrite(MOTOR2_IN2,  LOW);
    digitalWrite(MOTOR2_EN,   HIGH);
    delay(5000);                           // pause de 5s
    digitalWrite(MOTOR1_IN1,  LOW);        // moteur 1 dans l’autre sens
    digitalWrite(MOTOR1_IN2,  HIGH);
    digitalWrite(MOTOR1_EN,   HIGH);
    digitalWrite(MOTOR2_IN1,  LOW);        // moteur 2 dans l’autre sens
    digitalWrite(MOTOR2_IN2,  HIGH);
    digitalWrite(MOTOR2_EN,   HIGH);
    delay(5000);  
    digitalWrite(SORTIE_LED, LOW);
  } else {
    digitalWrite(MOTOR1_EN,   LOW);
    digitalWrite(MOTOR2_EN,   LOW);
    digitalWrite(SORTIE_LED, HIGH);
  }
}
