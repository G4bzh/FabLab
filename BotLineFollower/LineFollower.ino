/* Includes */

#include <AFMotor.h>

/* Constant */

#define BLACK     2
#define SPEED     150
#define MAP       9

#define Kp        1
#define Ki        100
#define Kd        1
#define TARGET    0

/* Sensors
 *
 * 0 is far right
 */

int sensor0 = 0;
int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;
int sensor4 = 0;

/* Map values translate sensor value in a 0-MAP range */
int map0 = 0;
int map1 = 0;
int map2 = 0;
int map3 = 0;
int map4 = 0;

/* Value Black and White */

int sensor0_white = 983;
int sensor1_white = 986;
int sensor2_white = 984;
int sensor3_white = 986;
int sensor4_white = 984;

int sensor0_black = 470;
int sensor1_black = 656;
int sensor2_black = 670;
int sensor3_black = 650;
int sensor4_black = 430;

/* PID value */

int last_error = 0;
int sum_error = 0;


/* Motors */

AF_DCMotor motor_left(3);
AF_DCMotor motor_right(4);



/* Read sensors and set map values */

int update_sensors()
{

  /* Sensor 0 */
  sensor0 = analogRead(A0);
  map0 = map(sensor0,sensor0_black,sensor0_white,0,MAP);
  map0 = constrain(map0,0,MAP);

  /* Sensor 1 */
  sensor1 = analogRead(A1);
  map1 = map(sensor1,sensor1_black,sensor1_white,0,MAP);
  map1 = constrain(map1,0,MAP);

  /* Sensor 2 */
  sensor2 = analogRead(A2);
  map2 = map(sensor2,sensor2_black,sensor2_white,0,MAP);
  map2 = constrain(map2,0,MAP);

  /* Sensor 3 */
  sensor3 = analogRead(A3);
  map3 = map(sensor3,sensor3_black,sensor3_white,0,MAP);
  map3 = constrain(map3,0,MAP);

  /* Sensor 4 */
  sensor4 = analogRead(A4);
  map4 = map(sensor4,sensor4_black,sensor4_white,0,MAP);
  map4 = constrain(map4,0,MAP);

  return (map4*10 + map3*5 + map1*-5 + map0*-10);
}


/* try to follow a black line */

void follow_line_basic()
{
  
  if (map2 < BLACK)
    {
        motor_left.setSpeed(SPEED);
	motor_left.run(BACKWARD);
	motor_right.setSpeed(SPEED);
	motor_right.run(BACKWARD);
    }
  else
    {
      if ( (map0 < BLACK) || (map1 < BLACK) )
	{
	  motor_left.setSpeed(SPEED);
	  motor_left.run(BACKWARD);
	  motor_right.setSpeed(0);
	  motor_right.run(RELEASE);
	}
      else if ( (map3 < BLACK) || (map4 < BLACK) )
	{
	  motor_left.setSpeed(0);
	  motor_left.run(RELEASE);
	  motor_right.setSpeed(SPEED);
	  motor_right.run(BACKWARD);
	}
    }

  return;
}

void follow_line_pid(unsigned long sensors)
{
  int error;
  int pid;
  int motor_left_speed;
  int motor_right_speed;

  error = TARGET - sensors;
  sum_error += error;
  pid =  error/Kp + sum_error/Ki + (error - last_error)/Kd ;
  last_error = error;
  
  motor_left_speed = SPEED - pid;
  motor_right_speed = SPEED + pid;
  
  motor_left_speed = constrain(motor_left_speed,0,255);
  motor_right_speed = constrain(motor_right_speed,0,255);
  
  
  motor_left.setSpeed(motor_left_speed);
  motor_left.run(BACKWARD);
  motor_right.setSpeed(motor_right_speed);
  motor_right.run(BACKWARD);

  delay(50);

   motor_left.run(RELEASE);
   motor_right.run(RELEASE);  

   delay(70);

  /* print map values */
  Serial.print("maps:");
  Serial.print(map4);
  Serial.print(",");
  Serial.print(map3);
  Serial.print(",");
  Serial.print(map2);
  Serial.print(",");
  Serial.print(map1);
  Serial.print(",");
  Serial.print(map0);
  Serial.print(" sensors:");
  Serial.print(sensors);
  Serial.print(" error:");
  Serial.print(error);
  Serial.print(" pid:");
  Serial.print(pid); 
  Serial.print(" L_speed:");
  Serial.print(motor_left_speed);
  Serial.print(" R_speed:");
  Serial.println(motor_right_speed);
  
  return;
}





/* Arduino Setup */

void setup() 
{
  /* Init Motors */
  motor_left.setSpeed(0);
  motor_left.run(RELEASE);
  motor_right.setSpeed(0);
  motor_right.run(RELEASE);

  /* Serial */
 Serial.begin(9600);
}



/* Arduino Loop */

void loop()
{
  /* Read sensors */
  unsigned long sensors = update_sensors();

  /* Adjust position */
  follow_line_pid(sensors);
  //follow_line_basic();
}
