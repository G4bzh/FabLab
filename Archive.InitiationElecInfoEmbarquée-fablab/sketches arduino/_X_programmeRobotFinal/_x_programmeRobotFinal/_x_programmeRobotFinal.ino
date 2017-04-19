#define VERSION     "\n\nrobotDom V1.0 - D.Valois\nUtilisation avec appli Android Joystick BT Commander"

/*
 * Cette application de pilotage de robot mobile utilise l'application Android Joystick BT COmmander configurée comme suit :
 * Bouton 1 : Sirène : active ou non la sirène
 * Bouton 2 : Autopilote : active ou non l'autopilotage
 * 
 * Les valeurs renvoyées sont les suivantes :
 * Champ 1 : valeur PWM moteur gauche
 * Champ 2 : valeur PMW moteur droit
 * Champ 3 : distance à l'obstacle
 */



#include "SoftwareSerial.h"
#include "timerOne.h"

#define DUREE_EVITEMENT   15       // durée de la manoeuvre d'évitement en 1OOeme de seconde


// définition des broches utilisées
#define US_ENTREE_ECHO      A0
#define US_SORTIE_TRIG      A1

#define ENTREE_INIT_RANDOM  2     // A2

#define SORTIE_BUZZER       A3 

#define MOTOR1_EN     5
#define MOTOR1_IN1    10
#define MOTOR1_IN2     9
#define MOTOR2_IN1     8
#define MOTOR2_IN2     7
#define MOTOR2_EN      6

#define    STX          0x02
#define    ETX          0x03
#define    ledPin       13
#define    SLOW         750                             // Datafields refresh rate (ms)
#define    FAST         250                             // Datafields refresh rate (ms)

SoftwareSerial mySerial(11,12);                         // BlueTooth module: pin#2=TX pin#3=RX
byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};                 // bytes received
byte buttonStatus = 0;                                  // first Byte sent to Android device

  int pwm_L, pwm_R;

int distanceObstacle;                                   // distance entre le robot et l'obstacle devant lui
boolean flagSireneActive = false;                       // mettre à true pour activer la sirène
boolean flagPiloteAuto = false;                         // mettre à true pour activer le pilotage automatique
int cap=0;                                              // cap suivi par le robot : 0 = tout droit, -255 gauche toute, 255 droite toute


void timer1Interrupt() {            // fonction appelée périodiquement
  static int freq = 500;
  static char pas = 1;

  freq = freq + pas;
  if (freq > 1000) pas = -1;
  if (freq < 500)  pas = 1;    
  if (flagSireneActive) {
    tone(SORTIE_BUZZER, freq); 
  } else {
    noTone(SORTIE_BUZZER);     
  }
}

void setup()  {
  // configuration des broches utilisées pour le télémètre US
  pinMode(US_SORTIE_TRIG, OUTPUT);     
  digitalWrite(US_SORTIE_TRIG, LOW);
  pinMode(US_ENTREE_ECHO, INPUT);      

  pinMode(MOTOR1_EN,      OUTPUT); 
  pinMode(MOTOR1_IN1,     OUTPUT);      
  pinMode(MOTOR1_IN2,     OUTPUT);      
  pinMode(MOTOR2_EN,      OUTPUT); 
  pinMode(MOTOR2_IN1,     OUTPUT);      
  pinMode(MOTOR2_IN2,     OUTPUT);      

  pinMode(SORTIE_BUZZER,  OUTPUT);     // broche sur laquelle est branché le buzzer

  randomSeed(analogRead(ENTREE_INIT_RANDOM));
  
  // initialisation interruption Timer 1
  Timer1.initialize(1000);                // configure un timer de 1000us
  Timer1.attachInterrupt( timer1Interrupt );  // toutes les 1000us, la fonction myInterrupt sera appelée

  motorG(0);
  motorG(0);

  Serial.begin(115200);
  mySerial.begin(57600);                                // 57600 = max value for softserial
  pinMode(ledPin, OUTPUT);    
  Serial.println(VERSION);
  while(mySerial.available())  mySerial.read();         // empty RX buffer
 
}

void loop() {
  static long previousMillis = 0;                            
  long currentMillis = millis();
  
 if(mySerial.available())  {                           // data received from smartphone
   delay(2);
   cmd[0] =  mySerial.read();  
   if(cmd[0] == STX)  {
//     Serial.print("STX : ");
     int i=1;      
     while(mySerial.available())  {
       delay(1);
       cmd[i] = mySerial.read();
//       Serial.print(cmd[i]);
//       Serial.print("-");
       if(cmd[i]>127 || i>7) {
//        Serial.println("error"); 
        break;     // Communication error
       }
       if((cmd[i]==ETX) && (i==2 || i==7)) {
//        Serial.println("ETX"); 
        break;     // Button or Joystick data
       }
       i++;
     }
     if     (i==2)          setButtonState(cmd[1]);    // 3 Bytes  ex: < STX "C" ETX >
     else if(i==7)          setJoystickState(cmd);     // 6 Bytes  ex: < STX "200" "180" ETX >
   }
 }
 
  currentMillis = millis();
  if(currentMillis - previousMillis > 100) {                        // on racalcule le cap toutes les 100ms
    previousMillis = currentMillis;
    getDistanceObstacle() ;
    sendBlueToothData();

    if (flagPiloteAuto) {
      autopilote();
    }
  }
}

void autopilote() {
  long randNumber;
  static boolean  evitementEnCours = false;
  static int compteurEvitement;
  static unsigned char diviseur=200;
  
  if ((!evitementEnCours) && (distanceObstacle > 15)) {    
    if (diviseur++ > 10) {
      diviseur = 0;
      if (cap < 0) randNumber = random(-90, 150);
      else randNumber = random(-150, 90);
    
      cap += randNumber;
      if (cap > 255) cap = 255;
      if (cap < -255) cap = -255;
      
      if (cap > 0) {
        pwm_L = 255;
        pwm_R = 255 - cap;
      } else {
        pwm_L = 255 + cap;
        pwm_R = 255;      
      }
      motorG(pwm_L);
      motorD(pwm_R);
  
      Serial.print("cap = ");
      Serial.print(cap);
      Serial.print(" -> ");
      Serial.print(pwm_L);
      Serial.print(" - ");
      Serial.println(pwm_R);
    } else {
      Serial.print("div = ");
      Serial.println((int)diviseur);
    }
    
 } else {
    if (!evitementEnCours) {
      // début d'évitement
      evitementEnCours = true;
      compteurEvitement = 0;
      randNumber = random(0,1);
      if (randNumber) {
        motorG(-127);        
        motorD(0);        
      } else {
        motorG(-127);        
        motorD(0);                
      }

      Serial.print("evitement = ");
      Serial.print(randNumber);
      
    } else {
      Serial.print(compteurEvitement);
      if (compteurEvitement++ > DUREE_EVITEMENT) {
        evitementEnCours = false; 
        Serial.println(" ");
      }
    }
  }  
}

void sendBlueToothData()  {
 static long previousMillis = 0;                            
 long currentMillis = millis();
 if(currentMillis - previousMillis > FAST) {   // send data back to smartphone
   previousMillis = currentMillis;

// Data frame transmitted back from Arduino to Android device:
// < 0X02   Buttons state   0X01   DataField#1   0x04   DataField#2   0x05   DataField#3    0x03 >  
// < 0X02      "01011"      0X01     "120.00"    0x04     "-4500"     0x05  "Motor enabled" 0x03 >    // example

   mySerial.print((char)STX);                                             // Start of Transmission
   mySerial.print(getButtonStatusString());  mySerial.print((char)0x1);   // buttons status feedback
   mySerial.print(pwm_L);                       mySerial.print((char)0x4);   // datafield #1
   mySerial.print(pwm_R);                       mySerial.print((char)0x5);   // datafield #2
   mySerial.print(distanceObstacle);            mySerial.print(" cm");                             // datafield #3
   mySerial.print((char)ETX);                                             // End of Transmission
 }  
}

String getButtonStatusString()  {
  String bStatus = "";
  for(int i=0; i<6; i++)  {
    if(buttonStatus & (B100000 >>i))      bStatus += "1";
    else                                  bStatus += "0";
  }
  return bStatus;
}



void setJoystickState(byte data[8])    {
  int tempInt;
  int tempVitesse;
  int joyX = (data[1]-48)*100 + (data[2]-48)*10 + (data[3]-48);       // obtain the Int from the ASCII representation
  int joyY = (data[4]-48)*100 + (data[5]-48)*10 + (data[6]-48);
  joyX = joyX - 200;                                                  // Offset to avoid
  joyY = joyY - 200;                                                  // transmitting negative numbers

  if (flagPiloteAuto) return;                                         // en mode autopilot, on ingore les commandes joystick

  if(joyX<-100 || joyX>100 || joyY<-100 || joyY>100)     return;      // commmunication error
  
  if(joyY >-10)  {                                                    // differential steering
    pwm_L = (joyY - (3*joyX)/4);
    pwm_R = (joyY + (3*joyX)/4);
  }  else  {
    pwm_L = (joyY + (3*joyX)/4);
    pwm_R = (joyY - (3*joyX)/4);
  }
  pwm_L = map(pwm_L, -100, 100, -255, 255);
  pwm_R = map(pwm_R, -100, 100, -255, 255);
  pwm_L = constrain(pwm_L, -255, 255);
  pwm_R = constrain(pwm_R, -255, 255);

 motorG(pwm_L);
 motorD(pwm_R);
/*  Serial.print("Moteurs : ");
  Serial.print(pwm_L);
  Serial.print(" - ");
  Serial.println(pwm_R);*/ 
}

void setButtonState(int bStatus)  {
 switch (bStatus) {
// -----------------  BUTTON #1  -----------------------
   case 'A':
     buttonStatus |= B000001;        // ON
     Serial.println("\n** Sirène : ON **");
     flagSireneActive = true;
     break;
   case 'B':
     buttonStatus &= B111110;        // OFF
     Serial.println("\n** Sirène : OFF **");
     flagSireneActive = false;
     break;

// -----------------  BUTTON #2  -----------------------
   case 'C':
     buttonStatus |= B000010;        // ON
     Serial.println("\n** Autopilote : ON **");
     flagPiloteAuto = true;
     break;
   case 'D':
     buttonStatus &= B111101;        // OFF
     Serial.println("\n** Autopilote : OFF **");
     flagPiloteAuto = false;
     break;

// -----------------  BUTTON #3  -----------------------
   case 'E':
     buttonStatus |= B000100;        // ON
     Serial.println("\n** Button_3: ON **");
     break;
   case 'F':
     buttonStatus &= B111011;      // OFF
     Serial.println("\n** Button_3: OFF **");
     break;

// -----------------  BUTTON #4  -----------------------
   case 'G':
     buttonStatus |= B001000;       // ON
     Serial.println("\n** Button_4: ON **");
     break;
   case 'H':
     buttonStatus &= B110111;    // OFF
     Serial.println("\n** Button_4: OFF **");
    break;

// -----------------  BUTTON #5  -----------------------
   case 'I':           // configured as momentary button
//      buttonStatus |= B010000;        // ON
     Serial.println("\n** Button_5: ++ pushed ++ **");
     break;
//   case 'J':
//     buttonStatus &= B101111;        // OFF
//     // your code...      
//     break;

// -----------------  BUTTON #6  -----------------------
   case 'K':
     buttonStatus |= B100000;        // ON
     Serial.println("\n** Button_6: ON **");
    break;
   case 'L':
     buttonStatus &= B011111;        // OFF
     Serial.println("\n** Button_6: OFF **");
     break;
 }
// ---------------------------------------------------------------
}


// met à jout le PWM du moteur gauche
void motorG(int vitesse)
{
  if (vitesse > 0) {
    digitalWrite(MOTOR1_IN1,  true);       
    digitalWrite(MOTOR1_IN2,  false);
  } else {
    digitalWrite(MOTOR1_IN1,  false);       
    digitalWrite(MOTOR1_IN2,  true);
  }
  analogWrite(MOTOR1_EN,    abs(vitesse));
}

// met à jout le PWM du moteur droit
void motorD(int vitesse)
{
  if (vitesse > 0) {
    digitalWrite(MOTOR2_IN1,  true);       
    digitalWrite(MOTOR2_IN2,  false);
  } else {
    digitalWrite(MOTOR2_IN1,  false);       
    digitalWrite(MOTOR2_IN2,  true);    
  }
  analogWrite(MOTOR2_EN,    abs(vitesse));
}

// calcule la distance à l'obstacle mesurée par le télémètre US
void getDistanceObstacle() {
  long lecture_echo;

  // Génération d'une impulsion de 10us
  digitalWrite(US_SORTIE_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_SORTIE_TRIG, LOW);
  
  lecture_echo = pulseIn(US_ENTREE_ECHO, HIGH);

/*    Serial.print("Distance : ");
  Serial.print(lecture_echo / 58);
  Serial.println("cm");*/

  distanceObstacle = (int)(lecture_echo / 58);
}

