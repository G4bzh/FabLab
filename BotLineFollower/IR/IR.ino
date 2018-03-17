void setup() {
 // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
 // put your main code here, to run repeatedly:
 // read the input on analog pin 0:
 int sensorValue0 = analogRead(A0);
 int sensorValue1 = analogRead(A1);
 int sensorValue2 = analogRead(A2);
 int sensorValue3 = analogRead(A3);
 int sensorValue4 = analogRead(A4);
 // print out the value you read:
 Serial.print("0:");
 Serial.print(sensorValue0);
 Serial.print(" 1:");
 Serial.print(sensorValue1);
 Serial.print(" 2:");
 Serial.print(sensorValue2);
 Serial.print(" 3:");
 Serial.print(sensorValue3);
 Serial.print(" 4:");
 Serial.println(sensorValue4);
 // delay in between reads for stability
 delay(1000);
}
