/* Utilisation du capteur Ultrason HC-SR04 */

// définition des broches utilisées
#define SORTIE_TRIG   A1
#define ENTREE_ECHO   A0

void setup()
{
  // configuration de la sortie utilisée pour générer le signal trig
  pinMode(SORTIE_TRIG, OUTPUT);     
  digitalWrite(SORTIE_TRIG, LOW);
  
  // configuration de l'entrée utilisée pour recevoir le signal echo
  pinMode(ENTREE_ECHO, INPUT);      

  Serial.begin(115200);             // initialisation de la liaison série
}

void loop()
{
  long lecture_echo;

  // Génération d'une impulsion de 10us
  digitalWrite(SORTIE_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SORTIE_TRIG, LOW);
  
  lecture_echo = pulseIn(ENTREE_ECHO, HIGH);
  Serial.print("Distance : ");
  Serial.print(lecture_echo / 58);
  Serial.println("cm");
  delay(1000);
}
