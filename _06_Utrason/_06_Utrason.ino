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
 
  pinMode(OUT_TRIG, OUTPUT);
  digitalWrite(OUT_TRIG, LOW);
  pinMode(IN_ECHO, INPUT);
  
  Serial.begin(9600);
 
}



void loop() {
  // put your main code here, to run repeatedly: 
  
  long read_echo;
  long dist;

  if (!stopNow)
  {
    
    // Generate a 10us pulse
    digitalWrite(OUT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(OUT_TRIG, LOW);
    
    // Read echo pulse timing
    read_echo = pulseIn(IN_ECHO, HIGH);
    dist = read_echo / 58;
    Serial.print("Distance : ");
    Serial.print(dist);
    Serial.println("cm");

    // Turn left backward if obstacle at 30cm or less 
    if (dist > 30)
    {
      move(FORWARD,STRAIGHT,230);
    }
    else
    {
      move(BACKWARD,LEFT,190);
    }
  
  }
}
