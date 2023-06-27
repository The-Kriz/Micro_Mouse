#include <PIDController.h>

// Motor A (Left Motor)
#define LEFT_PWM_PIN 12
#define LEFT_IN1_PIN 11
#define LEFT_IN2_PIN 10

// Motor B (Right Motor)
#define RIGHT_PWM_PIN 6
#define RIGHT_IN1_PIN 7
#define RIGHT_IN2_PIN 8

// Standby pin
#define STANDBY_PIN 9

// Encoder A and B pins for Left Motor
#define LEFT_ENCODER_PIN_A 31
#define LEFT_ENCODER_PIN_B 32

// Encoder A and B pins for Right Motor
#define RIGHT_ENCODER_PIN_A 0
#define RIGHT_ENCODER_PIN_B 1

// PID constants for Left Motor
#define LEFT_KP 1.00
#define LEFT_KD 0.60
#define LEFT_KI 0.00

// PID constants for Right Motor
#define RIGHT_KP 1.00
#define RIGHT_KD 0.50
#define RIGHT_KI 0.00

// Target distance to rotate motors (in encoder counts)
int targetDistanceLeft = 750;
int targetDistanceRight = 750;

volatile int encoderPosLeft = 0;
volatile int encoderPosRight = 0;
volatile boolean aStateLeft;
volatile boolean bStateLeft;
volatile boolean aStateRight;
volatile boolean bStateRight;

PIDController pidLeft;
PIDController pidRight;

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_PWM_PIN, OUTPUT);
  pinMode(LEFT_IN1_PIN, OUTPUT);
  pinMode(LEFT_IN2_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_IN1_PIN, OUTPUT);
  pinMode(RIGHT_IN2_PIN, OUTPUT);
  pinMode(STANDBY_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN_A), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN_A), rightEncoderISR, CHANGE);

  pidLeft.begin();
  pidLeft.tune(LEFT_KP, LEFT_KI, LEFT_KD);
  pidLeft.limit(30, 200);

  pidRight.begin();
  pidRight.tune(RIGHT_KP, RIGHT_KI, RIGHT_KD);
  pidRight.limit(30, 200);
}

void forward(int targetDistanceLeft, int targetDistanceRight) {
  pidLeft.setpoint(targetDistanceLeft);
  pidRight.setpoint(targetDistanceRight);
  bool notDone = true;
  while (notDone) {
    if (encoderPosLeft < targetDistanceLeft && encoderPosRight < targetDistanceRight) {
      digitalWrite(LEFT_IN1_PIN, HIGH);
      digitalWrite(LEFT_IN2_PIN, LOW);
      digitalWrite(RIGHT_IN1_PIN, HIGH);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      digitalWrite(STANDBY_PIN, HIGH);

      int motorSpeedLeft = pidLeft.compute(encoderPosLeft);
      int motorSpeedRight = pidRight.compute(encoderPosRight);

      Serial.print("Left Speed: ");
      Serial.print(motorSpeedLeft);
      Serial.print(" Right Speed: ");
      Serial.print(motorSpeedRight);
      Serial.print(" Left Count: ");
      Serial.print(encoderPosLeft);
      Serial.print(" Right Count: ");
      Serial.println(encoderPosRight);

      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    } else {
      encoderPosLeft -= targetDistanceLeft;
      encoderPosRight -= targetDistanceRight;
      analogWrite(LEFT_PWM_PIN, 0);
      analogWrite(RIGHT_PWM_PIN, 0);
      notDone = false;
    }
  }
}

void loop() {
  Serial.print("Left Count: ");
  Serial.print(encoderPosLeft);
  Serial.print(" Right Count: ");
  Serial.println(encoderPosRight);

  forward(targetDistanceLeft, targetDistanceRight);
  delay(2000);
  forward(targetDistanceLeft, targetDistanceRight);
}

void leftEncoderISR() {
  aStateLeft = digitalRead(LEFT_ENCODER_PIN_A);
  bStateLeft = digitalRead(LEFT_ENCODER_PIN_B);

  if (aStateLeft == bStateLeft)
    encoderPosLeft++;
  else
    encoderPosLeft--;
}

void rightEncoderISR() {
  aStateRight = digitalRead(RIGHT_ENCODER_PIN_A);
  bStateRight = digitalRead(RIGHT_ENCODER_PIN_B);

  if (aStateRight == bStateRight)
    encoderPosRight--;
  else
    encoderPosRight++;
}
