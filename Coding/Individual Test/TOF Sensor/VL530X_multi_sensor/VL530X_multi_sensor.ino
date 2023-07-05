#include <Wire.h>
#include <VL53L0X.h>

#define LeftTOF 3
#define CenterTOF 2
#define RightTOF 4

VL53L0X sensorL;
VL53L0X sensorC;
VL53L0X sensorR;

int a;
int b;
int c;

void setup()
{
  pinMode(LeftTOF, OUTPUT);
  pinMode(CenterTOF, OUTPUT);
  pinMode(RightTOF, OUTPUT);

  digitalWrite(LeftTOF, LOW);
  digitalWrite(CenterTOF, LOW);
  digitalWrite(RightTOF, LOW);

  delay(500);
  Wire.begin();
  Serial.begin (9600);

  Serial.println("L");
  digitalWrite(LeftTOF, HIGH);
  delay(150);
  sensorL.init(true);
  delay(100);
  sensorL.setAddress((uint8_t)01);

  Serial.println("C");
  digitalWrite(CenterTOF, HIGH);
  delay(150);
  sensorC.init(true);
  delay(100);
  sensorC.setAddress((uint8_t)02);

  Serial.println("R");
  digitalWrite(RightTOF, HIGH);
  delay(150);
  sensorR.init(true);
  delay(100);
  sensorR.setAddress((uint8_t)03);

  Serial.println("addresses set");

  sensorL.startContinuous();
  sensorC.startContinuous();
  sensorR.startContinuous();
}

void loop()
{
  Serial.print("L: ");
  a = sensorL.readRangeContinuousMillimeters();
  Serial.print(a);
  Serial.print(" C: ");
  b = sensorC.readRangeContinuousMillimeters();
  Serial.print(b);
  c = sensorR.readRangeContinuousMillimeters();
  Serial.print(" R: ");
  Serial.println(c);
  delay(1000);
}
