void setup() {
  // put your setup code here, to run once:

  // initialisation de la liaison série à 115200 bits/s
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  // affiche vers liaison série
  Serial.println("Salut les filles !!");

  // pause d'une seconde
  delay(1000);
}
