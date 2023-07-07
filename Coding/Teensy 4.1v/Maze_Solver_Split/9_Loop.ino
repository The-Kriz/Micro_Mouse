//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  while (!startButtonPressed)
  {
    if (digitalRead(START_BUTTON) == LOW)
    {
      startButtonPressed = true;
      Blink_Led();
      delay(2000);
      encoderPosLeft = 0;
      encoderPosRight = 0;
      //      Move_Forward(Home_cell_forward, Home_cell_forward);
      //      Move_Forward();
      //      Turn_Left();
      //      Turn_Right();
      //      if (Left_Wall())
      //      {
      //        Serial.println("L Wall");
      //        bt.println("L Wall");
      //      }
      //      if (Center_Wall())
      //      {
      //        Serial.println("C Wall");
      //        bt.println("C Wall");
      //      }
      //      if (Right_Wall())
      //      {
      //        Serial.println("R Wall");
      //        bt.println("R Wall");
      //      }
      //      bt.println("Done");
      RGB_LED("GREEN");
    }
  }
  //  Encoder_Readings();
  if (digitalRead(START_BUTTON) == LOW)
  {
    startButtonPressed = false;
  }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
