#include <PIDController.h>

// Encoder pins
#define LEFT_ENCODER_A 2
#define LEFT_ENCODER_B 4
#define RIGHT_ENCODER_A 3
#define RIGHT_ENCODER_B 5

// Motor pins
#define LEFT_MOTOR_CW 9
#define LEFT_MOTOR_CCW 10
#define RIGHT_MOTOR_CW 6
#define RIGHT_MOTOR_CCW 8
#define STBY 12


// PID constants
#define LEFT_KP 260.0
#define LEFT_KI 0.00
#define LEFT_KD 2000.0
#define RIGHT_KP 260.0
#define RIGHT_KI 0.00
#define RIGHT_KD 2000.0

// Target distance and motor speeds
int targetDistance = 100; // Target distance in encoder counts
int leftSpeed = 0;       // Motor speed for left wheel
int rightSpeed = 0;      // Motor speed for right wheel

// Encoder counts
volatile long int leftEncoderCount = 0;
volatile long int rightEncoderCount = 0;

// PID controllers
PIDController leftPID;
PIDController rightPID;

void setup() {
  Serial.begin(9600); // Serial for Debugging

  pinMode(LEFT_ENCODER_A, INPUT);
  pinMode(LEFT_ENCODER_B, INPUT);
  pinMode(RIGHT_ENCODER_A, INPUT);
  pinMode(RIGHT_ENCODER_B, INPUT);
  pinMode(STBY, OUTPUT);

  pinMode(LEFT_MOTOR_CW, OUTPUT);
  pinMode(LEFT_MOTOR_CCW, OUTPUT);
  pinMode(RIGHT_MOTOR_CW, OUTPUT);
  pinMode(RIGHT_MOTOR_CCW, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_A), leftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_A), rightEncoder, RISING);

  leftPID.begin();
  rightPID.begin();
  rightPID.tune(LEFT_KP, LEFT_KI, LEFT_KD);
  leftPID.tune(RIGHT_KP, RIGHT_KI, RIGHT_KD);
  leftPID.limit(-255, 255);
  rightPID.limit(-255, 255);

  leftPID.setpoint(targetDistance);
  rightPID.setpoint(targetDistance);
  digitalWrite(STBY, HIGH);
}

void loop() {
  leftSpeed = leftPID.compute(leftEncoderCount);
  rightSpeed = rightPID.compute(rightEncoderCount);

  setMotorSpeeds(leftSpeed, rightSpeed);
  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" Right Speed: ");
  Serial.print(rightSpeed);
  Serial.print(" Left Count: ");
  Serial.print(leftEncoderCount);
  Serial.print(" Right Count: ");
  Serial.println(rightEncoderCount);


}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  if (leftSpeed > 0) {
    analogWrite(LEFT_MOTOR_CW, leftSpeed);
    analogWrite(LEFT_MOTOR_CCW, 0);
  } else {
    analogWrite(LEFT_MOTOR_CW, 0);
    analogWrite(LEFT_MOTOR_CCW, abs(leftSpeed));
  }

  if (rightSpeed > 0) {
    analogWrite(RIGHT_MOTOR_CW, rightSpeed);
    analogWrite(RIGHT_MOTOR_CCW, 0);
  } else {
    analogWrite(RIGHT_MOTOR_CW, 0);
    analogWrite(RIGHT_MOTOR_CCW, abs(rightSpeed));
  }
}

void leftEncoder() {
  if (digitalRead(LEFT_ENCODER_B) == HIGH)
    leftEncoderCount++;
  else
    leftEncoderCount--;
}

void rightEncoder() {
  if (digitalRead(RIGHT_ENCODER_B) == HIGH)
    rightEncoderCount++;
  else
    rightEncoderCount--;
}
