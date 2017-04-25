#define OUT_BUZZER  A3

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_BUZZER,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;

  for(i=500; i<1000; i++)
  {
    tone(OUT_BUZZER,i);
    delay(1);
  }

  for(i=1000; i>500; i--)
  {
    tone(OUT_BUZZER,i);
    delay(1);
  }
}
