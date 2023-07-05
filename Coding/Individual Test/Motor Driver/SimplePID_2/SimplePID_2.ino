#include <PIDController.h>

#define PWM1 9
#define AIN1 10
#define AIN2 11

#define STBY 12

#define PWM2 6
#define BIN1 7
#define BIN2 8

#define encoderPinA1 2
#define encoderPinB1 5

#define encoderPinA2 3
#define encoderPinB2 4

#define LEFT_KP 1.00
#define LEFT_KD 0.60
#define LEFT_KI 0.00

#define RIGHT_KP 1.00
#define RIGHT_KD 0.50
#define RIGHT_KI 0.00


int done = 0;
bool happy = true;


volatile int encoderPosL = 0;
volatile int encoderPosR = 0;
volatile boolean aState1;
volatile boolean bState1;
volatile boolean aState2;
volatile boolean bState2;

PIDController pidL;
PIDController pidR;

int FDtargetDistanceL = 750; // Target distance to rotate motors (in encoder counts)  720 for cell to cell 250 rotate
int FDtargetDistanceR = 750;
void setup() {
  Serial.begin(9600);
  pinMode(PWM1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA1), encoderISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinA2), encoderISR2, CHANGE);

  pidL.begin();
  pidL.tune(LEFT_KP, LEFT_KI, LEFT_KD);
  pidL.limit(30, 200);

  pidR.begin();
  pidR.tune(RIGHT_KP, RIGHT_KI, RIGHT_KD);
  pidR.limit(30, 200);
}



void forward(int targetDistanceL, int targetDistanceR) {
  pidL.setpoint(targetDistanceL);
  pidR.setpoint(targetDistanceR);
  bool notDone = true;
  while (notDone) {
    if (encoderPosL < targetDistanceL && encoderPosR < targetDistanceR) {   // Both motors haven't reached the target distance yet
      digitalWrite(AIN1, HIGH);                                             // Motor A Rotate Clockwise
      digitalWrite(AIN2, LOW);
      digitalWrite(STBY, HIGH);
      digitalWrite(BIN1, HIGH);                                             // Motor B Rotate AntiClockwise
      digitalWrite(BIN2, LOW);

      int motorSpeed1 = pidL.compute(encoderPosL);
      int motorSpeed2 = pidR.compute(encoderPosR);

      Serial.print("Left Speed: ");
      Serial.print(motorSpeed1);
      Serial.print(" Right Speed: ");
      Serial.print(motorSpeed2);
      Serial.print(" ");
      Serial.print("Left Count: ");
      Serial.print(encoderPosL);
      Serial.print(" Right Count: ");
      Serial.println(encoderPosR);


      analogWrite(PWM1, abs(motorSpeed1));
      analogWrite(PWM2, abs(motorSpeed2));
    }
    else {
      encoderPosL = encoderPosL - 750;
      encoderPosR = encoderPosR - 750;
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 0);
      notDone = false;
    }
  }
}


void loop() {

  Serial.print(" Left Count: ");
  Serial.print(encoderPosL);
  Serial.print(" Right Count: ");
  Serial.println(encoderPosR);
  if (happy) {
    forward(FDtargetDistanceL, FDtargetDistanceR);
    delay(2000);
    forward(FDtargetDistanceL, FDtargetDistanceR);
    happy = false;
  }
}

void encoderISR1() {
  aState1 = digitalRead(encoderPinA1);
  bState1 = digitalRead(encoderPinB1);

  if (aState1 == bState1)
    encoderPosL++;
  else
    encoderPosL--;
}

void encoderISR2() {
  aState2 = digitalRead(encoderPinA2);
  bState2 = digitalRead(encoderPinB2);

  if (aState2 == bState2)
    encoderPosR--;
  else
    encoderPosR++;
}
