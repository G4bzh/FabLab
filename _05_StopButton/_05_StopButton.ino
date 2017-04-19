#define MOTOR1_EN 5
#define MOTOR1_IN1 10
#define MOTOR1_IN2 9
#define MOTOR2_IN1 8
#define MOTOR2_IN2 7
#define MOTOR2_EN 6


#define BRAKE     0
#define FORWARD   1
#define BACKWARD  2

#define STRAIGHT  0
#define LEFT      1
#define RIGHT     2

bool stopNow = false;

void move(int dir, int turn, int speed)
{
  
  if (dir == BRAKE)
  {
      // Deux moteurs dans le meme sens
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, HIGH);
      analogWrite(MOTOR1_EN, speed); 
      analogWrite(MOTOR2_EN, speed);
      
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
  stopNow = !stopNow;
  move(BRAKE,STRAIGHT,0);
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
}



void loop() {
  // put your main code here, to run repeatedly: 

  if (!stopNow) 
  {
    move(FORWARD,STRAIGHT,230);
    delay(2000);
    move(BRAKE,STRAIGHT,0);
    delay(300);
    move(FORWARD,RIGHT,190); 
    delay(2000);
    move(BRAKE,STRAIGHT,0);
    delay(300);
  }
  
}
