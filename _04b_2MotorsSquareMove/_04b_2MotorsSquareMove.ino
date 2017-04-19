#define MOTOR1_EN 5
#define MOTOR1_IN1 10
#define MOTOR1_IN2 9
#define MOTOR2_IN1 8
#define MOTOR2_IN2 7
#define MOTOR2_EN 6


#define FORWARD   0
#define LEFT      1
#define RIGHT     2
#define BRAKE     3
#define BACKWARD  4

void move(int dir, int speed)
{
  switch(dir)
  {
    case FORWARD:
    {
      // Deux moteurs dans le meme sens
      digitalWrite(MOTOR1_IN1, HIGH); 
      digitalWrite(MOTOR1_IN2, LOW);
      digitalWrite(MOTOR2_IN1, HIGH); 
      digitalWrite(MOTOR2_IN2, LOW);
      analogWrite(MOTOR1_EN, speed); 
      analogWrite(MOTOR2_EN, speed);
      
      break;
    }

    case LEFT:
    {
      // Moteur 2 STOP, Moteur 1 GO
      digitalWrite(MOTOR2_EN, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, LOW);
      
      digitalWrite(MOTOR1_IN1, HIGH); 
      digitalWrite(MOTOR1_IN2, LOW);
      analogWrite(MOTOR1_EN, speed); 
      
      break;
    }

    case RIGHT:
    {
      // Moteur 1 STOP, Moteur 2 GO
      digitalWrite(MOTOR1_EN, HIGH);
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, LOW);
      
      digitalWrite(MOTOR2_IN1, HIGH); 
      digitalWrite(MOTOR2_IN2, LOW);
      analogWrite(MOTOR1_EN, speed); 
      break;
    }

    case BRAKE:
    {
      digitalWrite(MOTOR1_EN, HIGH); 
      digitalWrite(MOTOR2_EN, HIGH);
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, LOW);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, LOW);

      break;
    }

    case BACKWARD:
    {
      // Deux moteurs dans le meme sens
      digitalWrite(MOTOR1_IN1, LOW); 
      digitalWrite(MOTOR1_IN2, HIGH);
      digitalWrite(MOTOR2_IN1, LOW); 
      digitalWrite(MOTOR2_IN2, HIGH);
      analogWrite(MOTOR1_EN, speed); 
      analogWrite(MOTOR2_EN, speed); 

      break;
    }

    default:
    {
      break;
    }
    
  }
  
}


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  move(FORWARD,230);
  delay(2000);
  move(BRAKE,0);
  delay(300);
  move(RIGHT,150); 
  delay(2000);
  move(BRAKE,0);
  delay(300);
  
}
