#include <PID_v1.h>


// Motor A
#define PWM1 9
#define AIN1 10
#define AIN2 11
#define encoderPinA1 2
#define encoderPinB1 4

// Motor B
#define PWM2 6
#define BIN1 7
#define BIN2 8
#define encoderPinA2 3
#define encoderPinB2 5

volatile int encoderPos1 = 0;
volatile int encoderPos2 = 0;
volatile boolean aState1;
volatile boolean bState1;
volatile boolean aState2;
volatile boolean bState2;

// PID Parameters
double Kp = 0.4; // Proportional gain
double Ki = 0.0; // Integral gain
double Kd = 0.1; // Derivative gain

double targetSpeed = 200; // Target speed in encoder counts per second

// PID Objects
PID pid1(&encoderPos1, &targetSpeed, &motorSpeed1, Kp, Ki, Kd, DIRECT);
PID pid2(&encoderPos2, &targetSpeed, &motorSpeed2, Kp, Ki, Kd, DIRECT);

void setup() {
  // Motor A
  pinMode(PWM1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(encoderPinA1, INPUT_PULLUP);
  pinMode(encoderPinB1, INPUT_PULLUP);

  // Motor B
  pinMode(PWM2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(encoderPinA2, INPUT_PULLUP);
  pinMode(encoderPinB2, INPUT_PULLUP);

  // Attach encoder interrupts
  attachInterrupt(digitalPinToInterrupt(encoderPinA1), encoderISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinA2), encoderISR2, CHANGE);

  // Set PID output limits
  pid1.SetOutputLimits(0, 255);
  pid2.SetOutputLimits(0, 255);

  // Set PID sample time (in milliseconds)
  pid1.SetSampleTime(10);
  pid2.SetSampleTime(10);

  // Set PID direction
  pid1.SetControllerDirection(DIRECT);
  pid2.SetControllerDirection(DIRECT);

  // Start the PID calculations
  pid1.SetMode(AUTOMATIC);
  pid2.SetMode(AUTOMATIC);
}

void loop() {
  // Calculate the target encoder positions based on the target speed
  double targetPosition = targetSpeed * (millis() / 1000.0);

  // Update the PID setpoints
  pid1.Setpoint = targetPosition;
  pid2.Setpoint = targetPosition;

  // Calculate the motor speeds using PID
  pid1.Compute();
  pid2.Compute();

  // Apply the motor speeds
  analogWrite(PWM1, motorSpeed1);
  analogWrite(PWM2, motorSpeed2);
}

void encoderISR1() {
  aState1 = digitalRead(encoderPinA1);
  bState1 = digitalRead(encoderPinB1);

  if (aState1 == bState1)
    encoderPos1++;
  else
    encoderPos1--;
}

void encoderISR2() {
  aState2 = digitalRead(encoderPinA2);
  bState2 = digitalRead(encoderPinB2);

  if (aState2 == bState2)
    encoderPos2++;
  else
    encoderPos2--;
}
