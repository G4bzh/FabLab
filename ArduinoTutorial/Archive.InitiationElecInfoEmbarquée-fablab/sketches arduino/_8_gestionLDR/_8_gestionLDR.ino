void setup() {
  
  pinMode(A5, INPUT);       //met la broche en entrée 
  pinMode(3, OUTPUT);       //met la broche en sortie
  Serial.begin(115200);     //Begin serial communication

}

void loop() {

  
  Serial.println(analogRead(A5));      // Envoie la valeur mesurée au moniteur série
  analogWrite(3, analogRead(A5)/4);    // valeur mesurée de 0 à 1023, mais                                  
                                       // paramètre analogRead de 0 à 255 -> /4
  delay(10); 
}
