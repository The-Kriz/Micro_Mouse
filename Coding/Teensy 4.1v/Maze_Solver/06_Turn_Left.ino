////////////////////////////////////////////////////////////////////////////////////////////////////////
void Turn_Left(int targetDistanceRight = LeftrotationTargetDistanceRight, int targetDistanceLeft = LeftrotationTargetDistanceLeft)
{
  Move_Forward(TurnForwardTargetDistance, TurnForwardTargetDistance);
  delay(200);
  pidLeft.setpoint(targetDistanceLeft);
  pidRight.setpoint(targetDistanceRight);
  bool Both_Done = false;
  bool Right_Done = false;
  bool Left_Done = false;
  digitalWrite(STANDBY_PIN, HIGH);
  while (!Both_Done)
  {
    if ((encoderPosRight < (targetDistanceRight + 5) and encoderPosRight > (targetDistanceRight - 5)) and Right_Done != true)
    {
      analogWrite(RIGHT_PWM_PIN, 0);
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      Right_Done = true;
    }
    else if (Right_Done != true)
    {
      int motorSpeedRight = pidRight.compute(encoderPosRight);
      if (encoderPosRight < targetDistanceRight)
      {
        digitalWrite(RIGHT_IN1_PIN, HIGH);
        digitalWrite(RIGHT_IN2_PIN, LOW);
      }
      else
      {
        digitalWrite(RIGHT_IN1_PIN, LOW);
        digitalWrite(RIGHT_IN2_PIN, HIGH);
      }
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
    if (encoderPosLeft == targetDistanceLeft)
    {
      analogWrite(LEFT_PWM_PIN, 0);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, LOW);
      Left_Done = true;
    }
    else if (Left_Done != true)
    {
      int motorSpeedLeft = pidLeft.compute(encoderPosLeft);
      if (encoderPosLeft < targetDistanceLeft)
      {
        digitalWrite(LEFT_IN1_PIN, HIGH);
        digitalWrite(LEFT_IN2_PIN, LOW);
      }
      else
      {
        digitalWrite(LEFT_IN1_PIN, LOW);
        digitalWrite(LEFT_IN2_PIN, HIGH);
      }
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

    bt.println(" ");
    Serial.println(" ");
    if (Left_Done == true and Right_Done == true)
    {
      Both_Done = true;
      digitalWrite(STANDBY_PIN, LOW);
      encoderPosRight -= targetDistanceRight;
      encoderPosLeft -= targetDistanceLeft;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
