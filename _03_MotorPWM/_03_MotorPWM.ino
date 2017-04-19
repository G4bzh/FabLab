  
 void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
}

void loop() {

    /* Definit PWM a 150 */
    analogWrite(3,150);
    delay(1000);
    
    /* Redefinit PWN a 250 */
    analogWrite(3,250);
    delay(1000); 
}
