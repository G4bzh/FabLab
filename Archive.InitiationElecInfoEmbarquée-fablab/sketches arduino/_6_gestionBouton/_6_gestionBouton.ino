void setup() {
  pinMode(2, INPUT_PULLUP);   //met la broche en entrée en activant le pullup interne 
  pinMode(13, OUTPUT);        //met la broche en sortie

}

void loop() {
  bool etatEntree = digitalRead(2);
  if (etatEntree == HIGH) {
    // entrée high = niveau haut = bouton relâché
    digitalWrite(13, LOW);
  } else {
    // entrée low = niveau bas = bouton appuyé
    digitalWrite(13, HIGH);
  }
}
