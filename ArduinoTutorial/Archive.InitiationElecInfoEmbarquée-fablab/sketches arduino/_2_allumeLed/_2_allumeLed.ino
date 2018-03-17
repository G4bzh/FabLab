void setup() {
  // put your setup code here, to run once:

  pinMode(3, OUTPUT);       //met la broche en sortie   

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(3,HIGH);     // met la broche au niveau haut (5V) – allume la LED
  delay(500);               // pause de 500 ms
  digitalWrite(3,LOW);      // met la broche au niveau bas (0V) – éteint la LED
  delay(500);               // pause de 500 ms
}
