//////////////////////////////////////////////////////////////////////////////////////////////////////
bool Left_Wall()
{
  int Left_Sensor_Reading = sensorL.readRangeContinuousMillimeters();
  Serial.print("L: ");
  Serial.println(Left_Sensor_Reading);
  bt.print("L: ");
  bt.println(Left_Sensor_Reading);
  if (Left_Sensor_Reading > sensor_Lower_Limits[0] && Left_Sensor_Reading < sensor_Upper_Limits[0])
  {
    leds[0] = CRGB::Blue;
    FastLED.show();
    return 1;
  }
  else
  {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool Center_Wall()
{
  int Center_Sensor_Reading = sensorC.readRangeContinuousMillimeters();
  Serial.print("C: ");
  Serial.println(Center_Sensor_Reading);
  bt.print("C: ");
  bt.println(Center_Sensor_Reading);
  if (Center_Sensor_Reading > sensor_Lower_Limits[1] && Center_Sensor_Reading < sensor_Upper_Limits[1])
  {
    leds[0] = CRGB::Amethyst;
    FastLED.show();
    return 1;
  }
  else
  {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool Right_Wall()
{
  int Right_Sensor_Reading = sensorR.readRangeContinuousMillimeters();
  Serial.print("R: ");
  Serial.println(Right_Sensor_Reading);
  bt.print("R: ");
  bt.println(Right_Sensor_Reading);
  if (Right_Sensor_Reading > sensor_Lower_Limits[2] && Right_Sensor_Reading < sensor_Upper_Limits[2])
  {
    leds[0] = CRGB::BlueViolet;
    FastLED.show();
    return 1;
  }
  else
  {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void TOF_Readings()
{
  int Left_Sensor_Reading = sensorL.readRangeContinuousMillimeters();
  Serial.print("L: ");
  Serial.println(Left_Sensor_Reading);
  bt.print("L: ");
  bt.println(Left_Sensor_Reading);
  int Center_Sensor_Reading = sensorC.readRangeContinuousMillimeters();
  Serial.print("C: ");
  Serial.println(Center_Sensor_Reading);
  bt.print("C: ");
  bt.println(Center_Sensor_Reading);
  int Right_Sensor_Reading = sensorR.readRangeContinuousMillimeters();
  Serial.print("R: ");
  Serial.println(Right_Sensor_Reading);
  bt.print("R: ");
  bt.println(Right_Sensor_Reading);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
float Read_MPU()
{
  mpu.update();
  int z = mpu.getAngleZ();
  z = z - MPU_Error;
  Serial.print("Z_Angle:");
  Serial.println(z);
  //  bt.print("Z_Angle: ");
  //  bt.println(z);
  z = wrapAngle(z);
  return z;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void updateOrientation(bool turn_right) {
  if (turn_right) {
    current_orientation = (current_orientation + 1) % 4; // Rotate clockwise
  } else {
    current_orientation = (current_orientation + 3) % 4; // Rotate counterclockwise
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to wrap angle values between -180 and +180 degrees
float wrapAngle(float angle) {
  if (angle > 180.0) {
    angle -= 360.0;
  }
  else if (angle < -180.0) {
    angle += 360.0;
  }
  return angle;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////North: 0
////East: 1
////South: 2
////West: 3
int Turn_Angle()
{
  int Angle;
  if (current_orientation == 0)
  {
    Angle == North;
  }
  else if (current_orientation == 1)
  {
    Angle == East;
  }
  else if (current_orientation == 2)
  {
    Angle == South;
  }
  else if (current_orientation == 3)
  {
    Angle == West;
  }
  return Angle;
}
