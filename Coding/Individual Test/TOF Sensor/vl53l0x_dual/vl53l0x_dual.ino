#include <Wire.h>
#include <VL53L0X.h>

VL53L0X tof1;
VL53L0X tof2;
//VL53L0X tof3;
//VL53L0X tof4;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize the sensors with their respective addresses
  tof1.init(0x2D);
  tof2.init(0x2A);
  // tof3.init(0x2B);
  //  tof4.init(0x2C);

  // Additional setup code...
}

void loop() {
  // Read the distance from each sensor
  uint16_t  distance1 = tof1.readRangeSingleMillimeters();
  uint16_t distance2 = tof2.readRangeSingleMillimeters();
  //  uint16_t distance3 = tof3.readRangeSingleMillimeters();
  //  int  distance4 = tof4.readRangeSingleMillimeters();

  // Print the distances
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" mm, ");

  Serial.print("Distance 2: ");
  Serial.print(distance2);
  Serial.print(" mm, ");
  Serial.println("");

  //  Serial.print("Distance 3: ");
  //  Serial.print(distance3);
  //  Serial.print(" mm, ");

  //  Serial.print("Distance 4: ");
  //  Serial.print(distance4);
  //  Serial.println(" mm");

  // Additional code...
  delay(100);
}
