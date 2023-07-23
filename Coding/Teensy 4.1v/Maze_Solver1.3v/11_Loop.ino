//void loop() // measuring encoder count with reset function (start button)
//{
//  if (digitalRead(START_BUTTON) == LOW)
//  {
//    leds[0] = CRGB::Yellow;
//    FastLED.show();
//    Blink_Led();
//    delay(2000);
//    encoderPosLeft = 0;
//    encoderPosRight = 0;
//    Move_Forward(1000,100);
//  }
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//void loop()  // testing Random Decision Making
//{
//  while (!startButtonPressed)
//  {
////    Encoder_Readings();
////    bt.println(Read_MPU());
//    if (digitalRead(START_BUTTON) == LOW)
//    {
//      startButtonPressed = true;
//      leds[0] = CRGB::Yellow;
//      FastLED.show();
//      Blink_Led();
//      delay(2000);
//      encoderPosLeft = 0;
//      encoderPosRight = 0;
//      //      Move_Forward();
//      MPU_Move_Forward(1000,1000);
//      //      Turn_Right();
//      //      Move_Forward();
//
//      //      MPU_Move_Forward();
//      //      Turn_Left();
//      //      Move_Forward();
//
//      //      MPU_Move_Forward();
//    }
//  }
//
//
//  startButtonPressed = false;
//  delay(100);
//  //    Move_Forward(1000, 1000);
//  //  MPU_Move_Forward(500,500, target_angle);
//  //  float z = Read_MPU();
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//void loop()  // testing Random Decision Making
//{
//  while (!startButtonPressed)
//  {
//    if (digitalRead(START_BUTTON) == LOW)
//    {
//      startButtonPressed = true;
//      leds[0] = CRGB::Yellow;
//      FastLED.show();
//      Blink_Led();
//      delay(2000);
//      encoderPosLeft = 0;
//      encoderPosRight = 0;
//      Move_Forward(Home_cell_forward, Home_cell_forward);
//      makeDecision1();
//      makeDecision1();
//      makeDecision1();
//      makeDecision1();
//      makeDecision1();
//      makeDecision1();      //      makeDecision();
//      //      makeDecision();
//
//    }
////    Encoder_Readings();
//  }
//  startButtonPressed = false;
//  delay(100);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()  // testing Random Decision Making
{
  //      bt.println(Read_MPU());

  while (!startButtonPressed)
  {
    if (digitalRead(START_BUTTON) == LOW)
    {
      startButtonPressed = true;
      leds[0] = CRGB::Yellow;
      FastLED.show();
      Blink_Led();
      delay(2000);
      encoderPosLeft = 0;
      encoderPosRight = 0;
      MPU_Move_Forward(Home_cell_forward, Home_cell_forward);
    }

  }
  makeDecision1();
  if (digitalRead(START_BUTTON) == LOW)
  {
    startButtonPressed = false;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//void loop()
//{
//  while (!startButtonPressed)
//  {
//    if (digitalRead(START_BUTTON) == LOW)
//    {
//      leds[0] = CRGB::Yellow;
//      FastLED.show();
//      startButtonPressed = true;
//      Blink_Led();
//      delay(2000);
//      encoderPosLeft = 0;
//      encoderPosRight = 0;
//      leds[0] = CRGB::Green;
//      FastLED.show();
//    }
//  }
//  if (Left_Wall())
//  {
//    Serial.println("L Wall");
//    bt.println("L Wall");
//  }
//  if (Right_Wall())
//  {
//    Serial.println("R Wall");
//    bt.println("R Wall");
//  }
//  if (Center_Wall())
//  {
//    Serial.println("C Wall");
//    bt.println("C Wall");
//  }
//  leds[0] = CRGB::Black;
//  FastLED.show();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//void loop()
//{
//  while (!startButtonPressed)
//  {
//    if (digitalRead(START_BUTTON) == LOW)
//    {
//      leds[0] = CRGB::YellowGreen;
//      FastLED.show();
//      startButtonPressed = true;
//      Blink_Led();
//      delay(2000);
//      encoderPosLeft = 0;
//      encoderPosRight = 0;
//      leds[0] = CRGB::Green;
//      FastLED.show();
//      //      Move_Forward(Home_cell_forward, Home_cell_forward);
//      //      Move_Forward();
//      //      if (Left_Wall())
//      //      {
//      //        Serial.println("L Wall");
//      //        bt.println("L Wall");
//      //      }
//      //      if (Right_Wall())
//      //      {
//      //        Serial.println("R Wall");
//      //        bt.println("R Wall");
//      //      }
//      //      if (Center_Wall())
//      //      {
//      //        Serial.println("C Wall");
//      //        bt.println("C Wall");
//      //      }
//      //      bt.println("Done");
//    }
//  }
//  //  Encoder_Readings();
//  if (digitalRead(START_BUTTON) == LOW)
//  {
//    startButtonPressed = false;
//  }
//
//}
