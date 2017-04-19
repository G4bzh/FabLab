#define MOTOR1_EN     5
#define MOTOR1_IN1    10
#define MOTOR1_IN2     9
#define MOTOR2_IN1     8
#define MOTOR2_IN2     7
#define MOTOR2_EN      6

void setup() {
  // configure les sorties utilisées pour piloter les deux moteurs
  pinMode(MOTOR1_EN,      OUTPUT); 
  pinMode(MOTOR1_IN1,     OUTPUT);      
  pinMode(MOTOR1_IN2,     OUTPUT);      
  pinMode(MOTOR2_EN,      OUTPUT); 
  pinMode(MOTOR2_IN1,     OUTPUT);      
  pinMode(MOTOR2_IN2,     OUTPUT);      
 
}

void loop() {
  digitalWrite(MOTOR1_IN1,  HIGH);       // moteur 1 dans un sens
  digitalWrite(MOTOR1_IN2,  LOW);
  analogWrite(MOTOR1_EN,   250);
  digitalWrite(MOTOR2_IN1,  HIGH);       // moteur 2 dans un sens
  digitalWrite(MOTOR2_IN2,  LOW);
  analogWrite(MOTOR2_EN,   250);
  delay(5000);                           // pause de 5s
/*  digitalWrite(MOTOR1_IN1,  LOW);        // moteur 1 dans l’autre sens
  digitalWrite(MOTOR1_IN2,  HIGH);
  digitalWrite(MOTOR1_EN,   HIGH);
  digitalWrite(MOTOR2_IN1,  LOW);        // moteur 2 dans l’autre sens
  digitalWrite(MOTOR2_IN2,  HIGH);
  digitalWrite(MOTOR2_EN,   HIGH);
  delay(5000);  */
}
