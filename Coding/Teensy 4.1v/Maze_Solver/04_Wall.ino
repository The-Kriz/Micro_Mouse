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
    return true;
  }
  else
  {
    return false;
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
    return true;
  }
  else
  {
    return false;
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
    return true;
  }
  else
  {
    return false;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////