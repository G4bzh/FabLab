#include <SoftwareSerial.h>                                
#include "TimerOne.h"

#define OUT_BUZZER  A3

#define   RX    3 
#define   TX    4

#define   STX   0x02
#define   ETX   0x03


#define MOTOR1_EN    5
#define MOTOR1_IN1   10
#define MOTOR1_IN2   9
#define MOTOR2_IN1   8
#define MOTOR2_IN2   7
#define MOTOR2_EN    6

#define OUT_TRIG   13
#define IN_ECHO    12


#define BRAKE     0
#define FORWARD   1
#define BACKWARD  2

#define STRAIGHT  0
#define LEFT      1
#define RIGHT     2

bool stopNow = false;
byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};
char buttons[6] = {'0', '0', '0', '0', '0', '0' };
byte buttonStatus = 0;
bool buzzer = false;

SoftwareSerial BTSERIAL(RX, TX); 


void buzzInterrupt()
{

    static int freq = 500;
    static char step = 1;
  
    freq += step;
    
    if (freq > 1000)
    {
      step = -1;
    }
  
    if (freq < 500)
    {
      step = 1;
    }

    if (buzzer)
    {
      tone(OUT_BUZZER, freq);
    }
    else
    {
      noTone(OUT_BUZZER);
    }
  
  return;
}


void move(int dir, int turn, int speed)
{
  
  if ( (dir == BRAKE) || stopNow )
  {
      // Deux moteurs dans le meme sens
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, HIGH);
      analogWrite(MOTOR1_EN, 0); 
      analogWrite(MOTOR2_EN, 0);
      
      return;
  } 
  
  if (dir == FORWARD)
  {
      // Deux moteurs dans le meme sens+
      digitalWrite(MOTOR1_IN1, HIGH); 
      digitalWrite(MOTOR1_IN2, LOW);
      digitalWrite(MOTOR2_IN1, HIGH); 
      digitalWrite(MOTOR2_IN2, LOW);
      analogWrite(MOTOR1_EN, speed); 
      analogWrite(MOTOR2_EN, speed);
  }
  
  if (dir == BACKWARD)
  {
      // Deux moteurs dans le meme sens-
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, HIGH);
      analogWrite(MOTOR1_EN, speed); 
      analogWrite(MOTOR2_EN, speed);     
  }
  
  if (turn == LEFT)
  {
      // Moteur 2 STOP, Moteur 1 GO
      digitalWrite(MOTOR2_EN, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, LOW);
    }

  if (turn == RIGHT)
    {
      // Moteur 1 STOP, Moteur 2 GO
      digitalWrite(MOTOR1_EN, HIGH);
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, LOW);
    }

 
  return;
  
}


void buttonStop()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 
 /* If interrupts come faster than 200ms, assume it's a bounce and ignore */
 if (interrupt_time - last_interrupt_time > 200) 
 {
  stopNow = !stopNow;
  move(BRAKE,STRAIGHT,0);
 }
 
 last_interrupt_time = interrupt_time;
 return;
}



void BT_Button(int flag)
{
  switch (flag)
  {
    case 'A':
    {
       /* B1 ON */
       buttons[5] = '1'; 
       buzzer = true;
       Serial.println("B1 ON");
       break;
    }

    case 'B':
    {
       /* B1 OFF */
       buttons[5] = '0'; 
       buzzer = false;
       Serial.println("B1 OFF");
       break;
    }

    case 'C':
    {
       /* B2 ON */
       buttons[4] = '1';  
       Serial.println("B2 ON");
       break;
    }

    case 'D':
    {
       /* B2 OFF */
       buttons[4] = '0'; 
       Serial.println("B2 OFF");
       break;
    }

    case 'E':
    {
       /* B3 ON */
       buttons[3] = '1'; 
       Serial.println("B3 ON");
       break;
    }

    case 'F':
    {
       /* B3 OFF */
       buttons[3] = '0'; 
       Serial.println("B3 OFF");
       break;
    }

    case 'G':
    {
       /* B4 ON */
       buttons[2] = '1'; 
       Serial.println("B4 ON");
       break;
    }

    case 'H':
    {
       /* B4 OFF */
       buttons[2] = '0'; 
       Serial.println("B4 OFF");
       break;
    }

    case 'I':
    {
       /* B5 ON */
       buttons[1] = '1'; 
       Serial.println("B5 ON");
       break;
    }

    case 'J':
    {
       /* B5 OFF */
       buttons[1] = '0'; 
       Serial.println("B5 OFF");
       break;
    }

    case 'K':
    {
       /* B6 ON */
       buttons[0] = '1';  
       Serial.println("B6 ON");
       break;
    }

    case 'L':
    {
       /* B6 OFF */
       buttons[0] = '0';  
       Serial.println("B6 OFF");
       break;
    }
 
  }

  return;
  
}


void BT_Josytick(byte cmd[8])
{
  int X = (cmd[1]-48)*100 + (cmd[2]-48)*10 + (cmd[3]-48);       // obtain the Int from the ASCII representation
  int Y = (cmd[4]-48)*100 + (cmd[5]-48)*10 + (cmd[6]-48);
  X = X - 200;                                                  // Offset to avoid
  Y = Y - 200;                                                  // transmitting negative numbers

  if(X<-100 || X>100 || Y<-100 || Y>100)     return;      // commmunication error
 
  Serial.print("Joystick:  ");
  Serial.print(X);  
  Serial.print(", ");  
  Serial.println(Y); 

  

  if (( Y == 100 ) && ( (X > -20) && (X < 20) ))
  {
      move(FORWARD,STRAIGHT,200);
  }
  else if (( Y == -100 ) && ( (X > -20) && (X < 20) ))
  {
      move(BACKWARD,STRAIGHT,200);
  }
  else if (( X == 100 ) && ( (Y > -20) && (Y < 20) ))
  {
      move(FORWARD,RIGHT,200);
  }
  else if (( X == -100 ) && ( (Y > -20) && (Y < 20) ) )
  {
      move(FORWARD,LEFT,200);
  }
  else
  {
    move(BRAKE,STRAIGHT,0);
  }
  
  return;
}


void BT_Send(int dist)
{
 
 static long previousMillis = 0;                             
 long currentMillis = millis();

 // Tempo at 250ms
 if(currentMillis - previousMillis > 250) 
 { 

   previousMillis = currentMillis; 
   
   BTSERIAL.print((char)STX);
   BTSERIAL.print(buttons);  
   BTSERIAL.print((char)0x1);   
   BTSERIAL.print("Obstacle:");            
   BTSERIAL.print((char)0x4);   
   BTSERIAL.print(dist);          
   BTSERIAL.print((char)0x5);   
   BTSERIAL.print("cm"); 
   BTSERIAL.print((char)ETX);  
 }

 return;
}


void setup() {
  // put your setup code here, to run once:

  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  // Int 0 -> pin 2, Int 1 -> pin 3
  attachInterrupt(0, buttonStop, CHANGE);

  pinMode(OUT_BUZZER,OUTPUT);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(buzzInterrupt);
 
  pinMode(OUT_TRIG, OUTPUT);
  digitalWrite(OUT_TRIG, LOW);
  pinMode(IN_ECHO, INPUT);

  BTSERIAL.begin(9600);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  long read_echo;
  long dist;
  
  if( BTSERIAL.available() )  
  { 

                     
     delay(2);
     cmd[0] =  BTSERIAL.read();  
     
     if(cmd[0] == STX)  
     {
       i=1;    
       
       while(BTSERIAL.available())  
       {
       
         delay(1);
         cmd[i] = BTSERIAL.read();
         
         if ( (cmd[i] > 127) || (i > 7) )
         {
            break;     // Communication error
         }
  
         
         if ( (cmd[i]==ETX) && ( (i==2) || (i==7) ) )
         {
            break;     // Button or Joystick data
         }
         
         i++;
       }
  
       
       if (i==2)
       {
          BT_Button(cmd[1]);    // 3 Bytes  ex: < STX "C" ETX >
       }
       
       if(i==7)
       {
          BT_Josytick(cmd);     // 6 Bytes  ex: < STX "200" "180" ETX >
       }
       
     }
     
   } 



  if (!stopNow)
  {
    
    // Generate a 10us pulse
    digitalWrite(OUT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(OUT_TRIG, LOW);
    
    // Read echo pulse timing
    read_echo = pulseIn(IN_ECHO, HIGH);
    dist = read_echo / 58;
  
  }

   BT_Send(dist);
}
