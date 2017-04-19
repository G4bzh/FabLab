#define MOTOR1_EN     11
#define MOTOR1_IN1    10
#define MOTOR1_IN2     9
#define MOTOR2_IN1     8
#define MOTOR2_IN2     7
#define MOTOR2_EN      6

boolean arretUrgence = false;

// fonction appelée lorsque l'état de l'entrée 3 change
void arretUrgenceFct() {
  arretUrgence = true;
  digitalWrite(13, HIGH);                        
  digitalWrite(MOTOR1_EN,   LOW);
  digitalWrite(MOTOR2_EN,   LOW);    
}

void setup() {
  // configure les sorties utilisées pour piloter les deux moteurs
  pinMode(MOTOR1_EN,      OUTPUT); 
  pinMode(MOTOR1_IN1,     OUTPUT);      
  pinMode(MOTOR1_IN2,     OUTPUT);      
  pinMode(MOTOR2_EN,      OUTPUT); 
  pinMode(MOTOR2_IN1,     OUTPUT);      
  pinMode(MOTOR2_IN2,     OUTPUT);      

  pinMode(13, OUTPUT);       //met la broche en sortie
  digitalWrite(13,LOW);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), arretUrgenceFct, CHANGE);
}

void loop() {
  if (!arretUrgence) {
    digitalWrite(MOTOR1_IN1,  HIGH);       // moteur 1 dans un sens
    digitalWrite(MOTOR1_IN2,  LOW);
    digitalWrite(MOTOR1_EN,   HIGH);
    digitalWrite(MOTOR2_IN1,  HIGH);       // moteur 2 dans un sens
    digitalWrite(MOTOR2_IN2,  LOW);
    digitalWrite(MOTOR2_EN,   HIGH);
    delay(5000);                           // pause de 5s
  }
  if (!arretUrgence) {
    digitalWrite(MOTOR1_IN1,  LOW);        // moteur 1 dans l’autre sens
    digitalWrite(MOTOR1_IN2,  HIGH);
    digitalWrite(MOTOR1_EN,   HIGH);
    digitalWrite(MOTOR2_IN1,  LOW);        // moteur 2 dans l’autre sens
    digitalWrite(MOTOR2_IN2,  HIGH);
    digitalWrite(MOTOR2_EN,   HIGH);
    delay(5000);  
  }
}

