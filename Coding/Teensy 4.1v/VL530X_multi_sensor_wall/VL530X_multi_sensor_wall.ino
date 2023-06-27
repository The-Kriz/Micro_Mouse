#include <Wire.h>
#include <VL53L0X.h>

#define LeftTOF 30
#define CenterTOF 5
#define RightTOF 4

int sensorLimits[3] = {100, 100, 100};  // Custom limits for each sensor (L, C, R)

VL53L0X sensorL;
VL53L0X sensorC;
VL53L0X sensorR;


void setup()
{
  pinMode(LeftTOF, OUTPUT);
  pinMode(CenterTOF, OUTPUT);
  pinMode(RightTOF, OUTPUT);

  digitalWrite(LeftTOF, LOW);
  digitalWrite(CenterTOF, LOW);
  digitalWrite(RightTOF, LOW);

  delay(500);

  Wire2.begin();

  Serial.begin(9600);

  Serial.println("L");
  digitalWrite(LeftTOF, HIGH);
  delay(150);
  sensorL.setBus(&Wire2);
  sensorL.init(true);
  delay(100);
  sensorL.setAddress((uint8_t)1);

  Serial.println("C");
  digitalWrite(CenterTOF, HIGH);
  delay(150);
  sensorC.setBus(&Wire2);
  sensorC.init(true);
  delay(100);
  sensorC.setAddress((uint8_t)2);

  Serial.println("R");
  digitalWrite(RightTOF, HIGH);
  delay(150);
  sensorR.setBus(&Wire2);
  sensorR.init(true);
  delay(100);
  sensorR.setAddress((uint8_t)3);

  Serial.println("Addresses set");

  sensorL.startContinuous();
  sensorC.startContinuous();
  sensorR.startContinuous();
}

void checkWall(int arr[], int wall[])
{
  for (int i = 0; i < 3; i++)
  {
    Serial.print(arr[i]);
    Serial.print(" ");

    if (arr[i] < sensorLimits[i])
    {
      wall[i] = 1;
    }
    else
    {
      wall[i] = 0;
    }
  }
}


void loop()
{
  int sensorReading[3];
  sensorReading[0] = sensorL.readRangeContinuousMillimeters();
  sensorReading[1] = sensorC.readRangeContinuousMillimeters();
  sensorReading[2] = sensorR.readRangeContinuousMillimeters();
  int wall[3];
  Serial.println("");
  checkWall(sensorReading, wall);
  for (int i = 0; i < 3; i++)
  {
    Serial.print(wall[i]);
    Serial.print(" ");
  }
  delay(1000);
}
