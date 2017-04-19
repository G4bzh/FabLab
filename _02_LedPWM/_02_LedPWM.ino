 
 int8_t compteur;
 
 void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  compteur = 0;
  Serial.begin(115200);
}

void loop() {

  Serial.println(compteur);
  
  if (compteur < 10)
  {

    /* Definit PWM a 50 */
    analogWrite(3,50);
    delay(500);
    
    /* Redefinit PWN a 250 */
    analogWrite(3,250);
    delay(500); 

   compteur++;
    
  }
  else
  {
    /* Eteint la led */
    analogWrite(3,0);
  }
  
}
