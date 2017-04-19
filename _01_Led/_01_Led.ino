 void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);

}

void loop() {

  /* Allume la PIN 3 */
  digitalWrite(3,HIGH);
  delay(500);
  /* Eteint la PIN 3 */
  digitalWrite(3, LOW);
  delay(500);

}
