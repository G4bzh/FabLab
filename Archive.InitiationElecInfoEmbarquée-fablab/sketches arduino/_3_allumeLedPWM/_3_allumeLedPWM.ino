void setup() {
  // put your setup code here, to run once:

  pinMode(3, OUTPUT);       //met la broche en sortie   

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(3,50);        // période PWM = 50
  delay(500);               // pause de 500 ms
  analogWrite(3,250);       // période PWM = 250
  delay(500);               // pause de 500 ms
}
