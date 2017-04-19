#define VERSION     "\n\nAndroTest V2.0 - @kas2014\ndemo for V5.x App"

// V2.0  changed to pure ASCII Communication Protocol ** not backward compatible **
// V1.4  improved communication errors handling
// V1.3  renamed for publishing, posted on 09/05/2014
// V1.2  Text display   ** not backward compatible **
// V1.1  Integer display
// V1.0  6 buttons + 4 data char implemented

// Demo setup:
// Button #1 controls pin #13 LED
// Button #4 toggle datafield display rate
// Button #5 configured as "push" button (momentary)
// Other buttons display demo message

// Arduino pin#2 to TX BlueTooth module
// Arduino pin#3 to RX BlueTooth module
// make sure your BT board is set @57600 bps
// better remove SoftSerial for PWM based projects

// For Mega 2560:
// remove   #include "SoftwareSerial.h", SoftwareSerial mySerial(2,3);
// search/replace  mySerial  >> Serial1
// pin#18 to RX bluetooth module, pin#19 to TX bluetooth module

#include "SoftwareSerial.h"


// définition des broches utilisées
#define US_SORTIE_TRIG   A0
#define US_ENTREE_ECHO   A1

#define MOTOR1_EN     5
#define MOTOR1_IN1    10
#define MOTOR1_IN2     9
#define MOTOR2_IN1     8
#define MOTOR2_IN2     7
#define MOTOR2_EN      6

#define    STX          0x02
#define    ETX          0x03
#define    ledPin       13
#define    SLOW         750                            // Datafields refresh rate (ms)
#define    FAST         250                             // Datafields refresh rate (ms)

SoftwareSerial mySerial(11,12);                         // BlueTooth module: pin#2=TX pin#3=RX
byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};                 // bytes received
byte buttonStatus = 0;                                  // first Byte sent to Android device
long previousMillis = 0;                                // will store last time Buttons status was updated
long sendInterval = SLOW;                               // interval between Buttons status transmission (milliseconds)
String displayStatus = "xxxx";                          // message to Android device

int vitesseMoteurDroit = 0;
int vitesseMoteurGauche = 0;

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


  motorG(0,0);
  motorG(0,0);

  
  Serial.begin(115200);
  mySerial.begin(57600);                                // 57600 = max value for softserial
  pinMode(ledPin, OUTPUT);    
  Serial.println(VERSION);
  while(mySerial.available())  mySerial.read();         // empty RX buffer
 
}

void loop() {
  int distanceObstacle;
  
 if(mySerial.available())  {                           // data received from smartphone
   delay(2);
   cmd[0] =  mySerial.read();  
   if(cmd[0] == STX)  {
//     Serial.print("STX : ");
     int i=1;      
     while(mySerial.available())  {
       delay(1);
       cmd[i] = mySerial.read();
 //      Serial.print(cmd[i]);
 //      Serial.print("-");
       if(cmd[i]>127 || i>7) {
 //       Serial.println("error"); 
        break;     // Communication error
       }
       if((cmd[i]==ETX) && (i==2 || i==7)) {
 //       Serial.println("ETX"); 
        break;     // Button or Joystick data
       }
       i++;
     }
     if     (i==2)          getButtonState(cmd[1]);    // 3 Bytes  ex: < STX "C" ETX >
     else if(i==7)          getJoystickState(cmd);     // 6 Bytes  ex: < STX "200" "180" ETX >
   }
 }

 //distanceObstacle = getDistanceObstacle() ;
 sendBlueToothData();
}

void sendBlueToothData()  {
 static long previousMillis = 0;                            
 long currentMillis = millis();
 if(currentMillis - previousMillis > sendInterval) {   // send data back to smartphone
   previousMillis = currentMillis;

// Data frame transmitted back from Arduino to Android device:
// < 0X02   Buttons state   0X01   DataField#1   0x04   DataField#2   0x05   DataField#3    0x03 >  
// < 0X02      "01011"      0X01     "120.00"    0x04     "-4500"     0x05  "Motor enabled" 0x03 >    // example

   mySerial.print((char)STX);                                             // Start of Transmission
   mySerial.print(getButtonStatusString());  mySerial.print((char)0x1);   // buttons status feedback
   mySerial.print(vitesseMoteurGauche);            mySerial.print((char)0x4);   // datafield #1
   mySerial.print(vitesseMoteurDroit);          mySerial.print((char)0x5);   // datafield #2
   mySerial.print(displayStatus);                                         // datafield #3
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



void getJoystickState(byte data[8])    {
 int pwm_L, pwm_R;
 int tempInt;
  int tempVitesse;
 int joyX = (data[1]-48)*100 + (data[2]-48)*10 + (data[3]-48);       // obtain the Int from the ASCII representation
 int joyY = (data[4]-48)*100 + (data[5]-48)*10 + (data[6]-48);
 joyX = joyX - 200;                                                  // Offset to avoid
 joyY = joyY - 200;                                                  // transmitting negative numbers

  if(joyX<-100 || joyX>100 || joyY<-100 || joyY>100)     return;      // commmunication error
  
  // Your code here ...
 /* Serial.print("Joystick position:  ");
  Serial.print(joyX);  
  Serial.print(", ");  
  Serial.println(joyY);*/

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


  vitesseMoteurGauche = pwm_L;
  vitesseMoteurDroit = pwm_R;

 if (pwm_L > 0) {
   motorG(true,pwm_L);
 } else {
   motorG(false,pwm_L);
 }
 if (pwm_R > 0) {
   motorD(true,pwm_R);
 } else {
   motorD(false,pwm_R);
 }


  Serial.print("Moteurs : ");
  Serial.print(pwm_L);
  Serial.print(" - ");
  Serial.println(pwm_R);
  
   
}

void getButtonState(int bStatus)  {
 switch (bStatus) {
// -----------------  BUTTON #1  -----------------------
   case 'A':
     buttonStatus |= B000001;        // ON
     Serial.println("\n** Button_1: ON **");
     // your code...      
     displayStatus = "LED <ON>";
     Serial.println(displayStatus);
     digitalWrite(ledPin, HIGH);
     break;
   case 'B':
     buttonStatus &= B111110;        // OFF
     Serial.println("\n** Button_1: OFF **");
     // your code...      
     displayStatus = "LED <OFF>";
     Serial.println(displayStatus);
     digitalWrite(ledPin, LOW);
     break;

// -----------------  BUTTON #2  -----------------------
   case 'C':
     buttonStatus |= B000010;        // ON
     Serial.println("\n** Button_2: ON **");
     // your code...      
     displayStatus = "Button2 <ON>";
     Serial.println(displayStatus);
     break;
   case 'D':
     buttonStatus &= B111101;        // OFF
     Serial.println("\n** Button_2: OFF **");
     // your code...      
     displayStatus = "Button2 <OFF>";
     Serial.println(displayStatus);
     break;

// -----------------  BUTTON #3  -----------------------
   case 'E':
     buttonStatus |= B000100;        // ON
     Serial.println("\n** Button_3: ON **");
     // your code...      
     displayStatus = "Motor #1 enabled"; // Demo text message
     Serial.println(displayStatus);
     break;
   case 'F':
     buttonStatus &= B111011;      // OFF
     Serial.println("\n** Button_3: OFF **");
     // your code...      
     displayStatus = "Motor #1 stopped";
     Serial.println(displayStatus);
     break;

// -----------------  BUTTON #4  -----------------------
   case 'G':
     buttonStatus |= B001000;       // ON
     Serial.println("\n** Button_4: ON **");
     // your code...      
     displayStatus = "Datafield update <FAST>";
     Serial.println(displayStatus);
     sendInterval = FAST;
     break;
   case 'H':
     buttonStatus &= B110111;    // OFF
     Serial.println("\n** Button_4: OFF **");
     // your code...      
     displayStatus = "Datafield update <SLOW>";
     Serial.println(displayStatus);
     sendInterval = SLOW;
    break;

// -----------------  BUTTON #5  -----------------------
   case 'I':           // configured as momentary button
//      buttonStatus |= B010000;        // ON
     Serial.println("\n** Button_5: ++ pushed ++ **");
     // your code...      
     displayStatus = "Button5: <pushed>";
     break;
//   case 'J':
//     buttonStatus &= B101111;        // OFF
//     // your code...      
//     break;

// -----------------  BUTTON #6  -----------------------
   case 'K':
     buttonStatus |= B100000;        // ON
     Serial.println("\n** Button_6: ON **");
     // your code...      
      displayStatus = "Button6 <ON>"; // Demo text message
    break;
   case 'L':
     buttonStatus &= B011111;        // OFF
     Serial.println("\n** Button_6: OFF **");
     // your code...      
     displayStatus = "Button6 <OFF>";
     break;
 }
// ---------------------------------------------------------------
}


void motorG(boolean direction, int power)
{
  digitalWrite(MOTOR1_IN1,  direction);       
  digitalWrite(MOTOR1_IN2,  !direction);
  analogWrite(MOTOR1_EN,   power);
}

void motorD(boolean direction, int power)
{
  digitalWrite(MOTOR2_IN1,  direction);       
  digitalWrite(MOTOR2_IN2,  !direction);
  analogWrite(MOTOR2_EN,   power);
}

// retourne la distance à l'obstacle mesurée par le télémètre US
int getDistanceObstacle() {
  int lecture_echo;

  // Génération d'une impulsion de 10us
  digitalWrite(US_SORTIE_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_SORTIE_TRIG, LOW);
  
  lecture_echo = pulseIn(US_ENTREE_ECHO, HIGH);

  return (lecture_echo / 58);
}

