#define PWM1 9
#define AIN1 10
#define AIN2 11

#define STBY 12

#define PWM2 6
#define BIN1 7
#define BIN2 8

#define encoderPinA1 2
#define encoderPinB1 4

#define encoderPinA2 3
#define encoderPinB2 5

volatile int encoderPos1 = 0;
volatile int encoderPos2 = 0;
volatile boolean aState1;
volatile boolean bState1;
volatile boolean aState2;
volatile boolean bState2;
int targetDistance = 100; // Target distance to rotate motors (in encoder counts)
int motorSpeed = 50; // Motor speed (adjust as needed)

float Kp = 0.4; // Proportional gain
float Kd = 0.1; // Derivative gain
int lastError1 = 0;
int lastError2 = 0;

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
}

void loop() {
  if (encoderPos1 < targetDistance && encoderPos2 < targetDistance) {
    // Both motors haven't reached the target distance yet
    digitalWrite(AIN1, HIGH); // Motor A Rotate Clockwise
    digitalWrite(AIN2, LOW);
    digitalWrite(STBY, HIGH);
    digitalWrite(BIN1, HIGH); // Motor B Rotate AntiClockwise
    digitalWrite(BIN2, LOW);
    
    // Calculate the error
    int error1 = targetDistance - encoderPos1;
    int error2 = targetDistance - encoderPos2;
    
    // Calculate the control signal
    int controlSignal1 = Kp * error1 + Kd * (error1 - lastError1);
    int controlSignal2 = Kp * error2 + Kd * (error2 - lastError2);
    
    // Apply the control signal as motor speed
    analogWrite(PWM1, constrain(controlSignal1, 0, 255)); // Speed control of Motor A
    analogWrite(PWM2, constrain(controlSignal2, 0, 255)); // Speed control of Motor B
    
    // Store the current error for the next iteration
    lastError1 = error1;
    lastError2 = error2;
  } else {
    // Stop both motors
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
  }

  if (encoderPos1 >= targetDistance) {
    // Motor A has reached the target distance
    // Stop Motor A
    analogWrite(PWM1, 0);
  }

  if (encoderPos2 >= targetDistance) {
    // Motor B has reached the target distance
    // Stop Motor B
    analogWrite(PWM2, 0);
  }
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
