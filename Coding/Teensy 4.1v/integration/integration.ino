#include <Wire.h>
#include <VL53L0X.h>
#include <PIDController.h>
///////////////////////////////////////////////////////////////////////////////////////////////
//PCB Buttons
#define START_BUTTON 40
#define Speed_CONTROLL 38
///////////////////////////////////////////////////////////////////////////////////////////////
PIDController pidLeft;
PIDController pidRight;
VL53L0X sensorL;
VL53L0X sensorC;
VL53L0X sensorR;
HardwareSerial& bt(Serial3);

///////////////////////////////////////////////////////////////////////////////////////////////
// TOF SENSOR PINS
#define LeftTOF 30
#define CenterTOF 5
#define RightTOF 4
int sensor_Lower_Limits[3] = {100, 100, 100};
int sensor_Upper_Limits[3] = {220, 300, 220};
///////////////////////////////////////////////////////////////////////////////////////////////
// MOTOR DRIVER PINS
#define RIGHT_PWM_PIN 6 // Motor A (Right Motor)
#define RIGHT_IN1_PIN 7
#define RIGHT_IN2_PIN 8
#define LEFT_PWM_PIN 12 // Motor B (Left Motor)
#define LEFT_IN1_PIN 11
#define LEFT_IN2_PIN 10
#define STANDBY_PIN 9  // Standby pin
#define LEFT_ENCODER_PIN_A 32  // Left Motor Encoder
#define LEFT_ENCODER_PIN_B 31
#define RIGHT_ENCODER_PIN_A 0  // Right Motor Encoder
#define RIGHT_ENCODER_PIN_B 1
///////////////////////////////////////////////////////////////////////////////////////////////
// ENCODER VARIABLES
volatile int encoderPosLeft = 0;
volatile int encoderPosRight = 0;
volatile boolean aStateLeft;
volatile boolean bStateLeft;
volatile boolean aStateRight;
volatile boolean bStateRight;
///////////////////////////////////////////////////////////////////////////////////////////////
// PID VARIABLES
#define LEFT_KP  1.0500    //0.650  // PID constants for Left Motor
#define LEFT_KD  10.0000    //0.100
#define LEFT_KI  0.0000
#define RIGHT_KP 1.0800   //0.680  // PID constants for Right Motor
#define RIGHT_KD 10.000   //0.100
#define RIGHT_KI 0.0000
///////////////////////////////////////////////////////////////////////////////////////////////
// CELL TRAVEL DISTANCE (ENCODER COUNT)
int forwardTargetDistanceLeft = 410;      //140  // 726 total cell distance // Target distance to move forward (in encoder counts)
int forwardTargetDistanceRight = 410;
int LeftrotationTargetDistanceLeft = 0;     // Target distance to rotate motors (in encoder counts)
int LeftrotationTargetDistanceRight = 537;  //550 rotation //29 per cm
int RightrotationTargetDistanceLeft = 537;    // Target distance to rotate motors (in encoder counts)
int RightrotationTargetDistanceRight = 0;
///////////////////////////////////////////////////////////////////////////////////////////////
// OTHER
bool startButtonPressed = false;
void setup()
{
  Serial.begin(9600);
  bt.begin(9600); // BT-MODULE (HC05)
  pinMode(START_BUTTON, INPUT_PULLUP);

  pinMode(LEFT_PWM_PIN, OUTPUT);  // MOTOR DRIVER
  pinMode(LEFT_IN1_PIN, OUTPUT);
  pinMode(LEFT_IN2_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_IN1_PIN, OUTPUT);
  pinMode(RIGHT_IN2_PIN, OUTPUT);
  pinMode(STANDBY_PIN, OUTPUT);
  pinMode(LeftTOF, OUTPUT);  // TOF
  pinMode(CenterTOF, OUTPUT);
  pinMode(RightTOF, OUTPUT);
  digitalWrite(LeftTOF, LOW);
  digitalWrite(CenterTOF, LOW);
  digitalWrite(RightTOF, LOW);


  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN_A), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN_A), rightEncoderISR, CHANGE);

  pidLeft.begin();
  pidLeft.tune(LEFT_KP, LEFT_KI, LEFT_KD);
  pidLeft.limit(30, 255);
  pidRight.begin();
  pidRight.tune(RIGHT_KP, RIGHT_KI, RIGHT_KD);
  pidRight.limit(30, 255);

  Wire2.begin();
  Serial.println("L");           // TOF ADDRESS SETUP
  bt.println("L");
  digitalWrite(LeftTOF, HIGH);
  delay(150);
  sensorL.setBus(&Wire2);
  sensorL.init(true);
  delay(100);
  sensorL.setAddress((uint8_t)1);
  Serial.println("C");
  bt.println("C");
  digitalWrite(CenterTOF, HIGH);
  delay(150);
  sensorC.setBus(&Wire2);
  sensorC.init(true);
  delay(100);
  sensorC.setAddress((uint8_t)2);
  Serial.println("R");
  bt.println("R");
  digitalWrite(RightTOF, HIGH);
  delay(150);
  sensorR.setBus(&Wire2);
  sensorR.init(true);
  delay(100);
  sensorR.setAddress((uint8_t)3);
  Serial.println("Addresses set");
  bt.println("Addresses set");
  sensorL.startContinuous();
  sensorC.startContinuous();
  sensorR.startContinuous();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void leftEncoderISR()
{
  aStateLeft = digitalRead(LEFT_ENCODER_PIN_A);
  bStateLeft = digitalRead(LEFT_ENCODER_PIN_B);

  if (aStateLeft == bStateLeft)
    encoderPosLeft++;
  else
    encoderPosLeft--;
}

void rightEncoderISR()
{
  aStateRight = digitalRead(RIGHT_ENCODER_PIN_A);
  bStateRight = digitalRead(RIGHT_ENCODER_PIN_B);

  if (aStateRight == bStateRight)
    encoderPosRight--;
  else
    encoderPosRight++;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
int getWallArrangementID(int wall[3]) {
  int id = 0;
  if (wall[0] == 0) {
    id += 4;
  }
  if (wall[1] == 0) {
    id += 2;
  }
  if (wall[2] == 0) {
    id += 1;
  }
  return id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void checkWall(int sensorReading[], int wall[])
{
  for (int i = 0; i < 3; i++)
  {
    //    Serial.print(arr[i]);
    //    Serial.print(" hi ");
    //    bt.print(arr[i]);
    //    bt.print(" ");

    if (sensorReading[i] > sensor_Lower_Limits[i] && sensorReading[i] < sensor_Upper_Limits[i])
    {
      wall[i] = 1;
    }
    else
    {
      wall[i] = 0;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
int checkDistance()
{
  int sensorReading[3] = {0, 0, 0};
  sensorReading[0] = sensorL.readRangeContinuousMillimeters();
  sensorReading[1] = sensorC.readRangeContinuousMillimeters();
  sensorReading[2] = sensorR.readRangeContinuousMillimeters();

  Serial.print("L: ");
  Serial.print(sensorReading[0]);
  Serial.print(" C: ");
  Serial.print(sensorReading[1]);
  Serial.print(" R: ");
  Serial.println(sensorReading[2]);

  bt.print("L: ");
  bt.print(sensorReading[0]);
  bt.print(" C: ");
  bt.print(sensorReading[1]);
  bt.print(" R: ");
  bt.println(sensorReading[2]);

  int wall[3];
  checkWall(sensorReading, wall);

  for (int i = 0; i < 3; i++)
  {
    Serial.print(wall[i]);
    Serial.print(" ");
    bt.print(wall[i]);
    bt.print(" ");
  }

  int Cell_ID = getWallArrangementID(wall);
  Serial.print("Cell ID: ");
  Serial.println(Cell_ID);
  bt.print("Cell ID: ");
  bt.println(Cell_ID);

  return Cell_ID;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void moveForward(int targetDistanceLeft, int targetDistanceRight)
{
  pidLeft.setpoint(targetDistanceLeft);
  pidRight.setpoint(targetDistanceRight);
  bool notDone = true;
  while (notDone)
  {
    if (encoderPosLeft <= targetDistanceLeft && encoderPosRight <= targetDistanceRight)
    {
      // Move forward
      digitalWrite(LEFT_IN1_PIN, HIGH);
      digitalWrite(LEFT_IN2_PIN, LOW);
      digitalWrite(RIGHT_IN1_PIN, HIGH);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      digitalWrite(STANDBY_PIN, HIGH);

      // Compute motor speeds using PID control
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
      bt.print("Left Speed: ");     // BT MODULE (RX3 TX3)
      bt.print(motorSpeedLeft);
      bt.print(" Right Speed: ");
      bt.print(motorSpeedRight);
      bt.print(" Left Count: ");
      bt.print(encoderPosLeft);
      bt.print(" Right Count: ");
      bt.println(encoderPosRight);

      // Set motor speeds
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    }
    else
    {
      // Reset encoder positions and stop motors
      encoderPosLeft -= targetDistanceLeft;
      encoderPosRight -= targetDistanceRight;
      analogWrite(LEFT_PWM_PIN, 0);
      analogWrite(RIGHT_PWM_PIN, 0);
      notDone = false;
    }
  }
  digitalWrite(STANDBY_PIN, LOW);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void turn(int targetDistanceLeft, int targetDistanceRight)
{
  pidLeft.setpoint(targetDistanceLeft);
  pidRight.setpoint(targetDistanceRight);
  bool notDone = true;
  bool Right_Done = false;
  bool Left_Done = false;

  digitalWrite(LEFT_IN1_PIN, HIGH);
  digitalWrite(LEFT_IN2_PIN, LOW);
  digitalWrite(RIGHT_IN1_PIN, HIGH);
  digitalWrite(RIGHT_IN2_PIN, LOW);
  digitalWrite(STANDBY_PIN, HIGH);

  while (notDone)
  {
    if (encoderPosLeft < targetDistanceLeft)
    {
      int motorSpeedLeft = pidLeft.compute(encoderPosLeft);
      Serial.print("Left Speed: ");
      Serial.print(motorSpeedLeft);
      Serial.print(" Left Count: ");
      Serial.print(encoderPosLeft);
      bt.print("Left Speed: ");     // BT MODULE (RX3 TX3)
      bt.print(motorSpeedLeft);
      bt.print(" Left Count: ");
      bt.print(encoderPosLeft);
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
    }
    else if (encoderPosLeft > targetDistanceLeft)
    {
      int motorSpeedLeft = pidLeft.compute(encoderPosLeft);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, HIGH);
      Serial.print("Left Speed: ");
      Serial.print(motorSpeedLeft);
      Serial.print(" Left Count: ");
      Serial.print(encoderPosLeft);
      bt.print("Left Speed: ");     // BT MODULE (RX3 TX3)
      bt.print(motorSpeedLeft);
      bt.print(" Left Count: ");
      bt.print(encoderPosLeft);
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
    }
    else
    {
      // Reset encoder positions and stop motors
      encoderPosLeft -= targetDistanceLeft;
      analogWrite(LEFT_PWM_PIN, 0);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, LOW);
      Left_Done = true;
    }
    if (encoderPosRight < targetDistanceRight)
    {
      int motorSpeedRight = pidRight.compute(encoderPosRight);

      Serial.print(" Right Speed: ");
      Serial.print(motorSpeedRight);
      Serial.print(" Right Count: ");
      Serial.println(encoderPosRight);
      bt.print(" Right Speed: "); // BT MODULE (RX3 TX3)
      bt.print(motorSpeedRight);
      bt.print(" Right Count: ");
      bt.print(encoderPosRight);

      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    }
    else if (encoderPosRight > targetDistanceRight)
    {
      int motorSpeedRight = pidRight.compute(encoderPosRight);
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, HIGH);
      Serial.print(" Right Speed: ");
      Serial.print(motorSpeedRight);
      Serial.print(" Right Count: ");
      Serial.println(encoderPosRight);
      bt.print(" Right Speed: "); // BT MODULE (RX3 TX3)
      bt.print(motorSpeedRight);
      bt.print(" Right Count: ");
      bt.print(encoderPosRight);

      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    }
    else
    {
      // Reset encoder positions and stop motors
      encoderPosRight -= targetDistanceRight;
      analogWrite(RIGHT_PWM_PIN, 0);
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      Right_Done = true;
    }
    bt.println(" ");
    Serial .println(" ");

    if (Left_Done == true and Right_Done == true)
    {
      notDone = false;
    }
  }

  digitalWrite(STANDBY_PIN, LOW);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void makeDecision(int Cell_ID) {
  switch (Cell_ID) {
    case 0:
      // Handle Cell ID 1 // 111
      bt.println("Cell ID 1 |111| ");
      moveForward(316, 316);
    case 1:
      // Handle Cell ID 1 // 110
      bt.println("Cell ID 1 |110| ");
      moveForward(316, 316);
      turn(RightrotationTargetDistanceLeft, RightrotationTargetDistanceRight);
      moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
      break;
    case 2:
      // Handle Cell ID 2 // 101
      bt.println("Cell ID 2 |101| ");
      moveForward(726, 726);
      break;
    case 3:
      // Handle Cell ID 3 // 100
      bt.println("Cell ID 3 |000| ");
      moveForward(316, 316);
      turn(RightrotationTargetDistanceLeft, RightrotationTargetDistanceRight);
      moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
      break;
    case 4:
      // Handle Cell ID 4  // 011
      bt.println("Cell ID 4 |011| ");
      moveForward(316, 316);
      turn(LeftrotationTargetDistanceLeft, LeftrotationTargetDistanceRight);
      moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
      break;
    case 5:
      // Handle Cell ID 5  // 010
      bt.println("Cell ID 5 |010| ");
      moveForward(316, 316);
      turn(LeftrotationTargetDistanceLeft, LeftrotationTargetDistanceRight);
      moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
      break;
    case 6:
      // Handle Cell ID 6 // 001
      bt.println("Cell ID 6 |001| ");
      moveForward(316, 316);
      turn(LeftrotationTargetDistanceLeft, LeftrotationTargetDistanceRight);
      moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
      break;
    case 7:
      bt.println("Cell ID 7 |000| "); // 000
      // Handle Cell ID 7
      break;
    default:
      // Handle unknown Cell ID or error condition
      break;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  //  while (!startButtonPressed) {
  if (digitalRead(START_BUTTON) == LOW) {
    //    startButtonPressed = true; // Press Button to START
    encoderPosLeft = 0;
    encoderPosRight = 0;
    delay(1500);
    //    turn(RightrotationTargetDistanceLeft, RightrotationTargetDistanceRight);
    turn(LeftrotationTargetDistanceLeft, LeftrotationTargetDistanceRight);
    //    moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
    //    int Cell_ID = checkDistance();
    //    makeDecision(Cell_ID);
    encoderPosLeft = 0;
    encoderPosRight = 0;
  }
  //  }

  Serial.print("Left Count: ");     // USB Serial
  Serial.print(encoderPosLeft);
  Serial.print(" Right Count: ");
  Serial.println(encoderPosRight);
  bt.print("Left Count: ");    // BT MODULE (RX3 TX3)
  bt.print(encoderPosLeft);
  bt.print(" Right Count: ");
  bt.println(encoderPosRight);


  //moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
  //turn(RightrotationTargetDistanceLeft, RightrotationTargetDistanceRight);
  //    moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);

  //  moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
  //  delay(1000);
  //  moveForward(forwardTargetDistanceLeft, forwardTargetDistanceRight);
  //  delay(2000);
  //  turn(1, -rotationTargetDistanceLeft, rotationTargetDistanceRight);
  //  delay(2000);
  //  turn(2, rotationTargetDistanceLeft, -rotationTargetDistanceRight);
  //  delay(1000);
  //  int Cell_ID = checkDistance();
  //  while (true) {
  //    Serial.println(Cell_ID);
  //    bt.println(Cell_ID);
  //  }
  //  delay(200);
}
