/*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Home

*/

#define PWM1 9
#define AIN1 5
#define AIN2 4
#define STBY 7


int out;

void setup() {
  Serial.begin(9600);
  pinMode(PWM1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
}

void loop() {

  digitalWrite(AIN1, HIGH); //Motor A Rotate Clockwise
  digitalWrite(AIN2, LOW);
  digitalWrite(STBY, HIGH);

  for (int i = 14 ; i <= 240; i++) {
    out = i ;
    Serial.println(out);
    analogWrite(PWM1, out); //Speed control of Motor A
    delay(500);
  }
}
