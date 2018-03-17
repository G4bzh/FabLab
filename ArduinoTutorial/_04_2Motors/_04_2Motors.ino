#define MOTOR1_EN 5
#define MOTOR1_IN1 10
#define MOTOR1_IN2 9
#define MOTOR2_IN1 8
#define MOTOR2_IN2 7
#define MOTOR2_EN 6

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(MOTOR1_IN1, HIGH); // moteur 1 dans un sens
  digitalWrite(MOTOR1_IN2, LOW);
  //digitalWrite(MOTOR1_EN, HIGH);
  analogWrite(MOTOR1_EN, 100); // Vitesse de 100 
  
  digitalWrite(MOTOR2_IN1, HIGH); // moteur 2 dans un sens
  digitalWrite(MOTOR2_IN2, LOW);
  //digitalWrite(MOTOR2_EN, HIGH);
  analogWrite(MOTOR2_EN, 250); // Vitesse 250
  
  delay(5000); // pause de 5s
  
  digiitalWrite(MOTOR1_IN1, LOW); // moteur 1 dans l’autre sens
  digitalWrite(MOTOR1_IN2, HIGH);
  //digitalWrite(MOTOR1_EN, HIGH);
  analogWrite(MOTOR1_EN, 10 0); // Vitesse de 50 

  digitalWrite(MOTOR2_IN1, LOW); // moteur 2 dans l’autre sens
  digitalWrite(MOTOR2_IN2, HIGH);
  //digitalWrite(MOTOR2_EN, HIGH);
  analogWrite(MOTOR2_EN, 250); // Vitesse 250
  
  delay(5000); // pause de 5s
}
