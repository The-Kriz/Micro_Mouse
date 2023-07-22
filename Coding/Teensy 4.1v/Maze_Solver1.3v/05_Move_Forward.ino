////////////////////////////////////////////////////////////////////////////////////////////////////////
void Move_Forward(int targetDistanceLeft = forwardTargetDistance, int targetDistanceRight = forwardTargetDistance)
{
  pid_Forward_Left.setpoint(targetDistanceLeft);
  pid_Forward_Right.setpoint(targetDistanceRight);
  bool Both_Done = false;
  bool Right_Done = false;
  bool Left_Done = false;
  while (!Both_Done)
  {

    Serial.print("Left_Count: ");     // USB Serial
    Serial.print(encoderPosLeft);
    Serial.print(" Right_Count: ");
    Serial.println(encoderPosRight);
    bt.print(",");    // BT MODULE (RX3 TX3)
    bt.print("Left_Count:");    // BT MODULE (RX3 TX3)
    bt.print(encoderPosLeft);
    bt.print(",");
    bt.print(" Right_Count:");
    bt.println(encoderPosRight);
    if ((encoderPosLeft < (targetDistanceLeft + 5) && encoderPosLeft > (targetDistanceLeft - 5)) and Left_Done != true)
    {
      analogWrite(LEFT_PWM_PIN, 0);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, LOW);
      Left_Done = true;
    }
    else if (Left_Done != true)
    {
      int motorSpeedLeft = pid_Forward_Left.compute(encoderPosLeft);
      if ( encoderPosLeft < targetDistanceLeft )
      {
        digitalWrite(LEFT_IN1_PIN, HIGH);
        digitalWrite(LEFT_IN2_PIN, LOW);
      }
      else
      {
        digitalWrite(LEFT_IN1_PIN, LOW);
        digitalWrite(LEFT_IN2_PIN, HIGH);
      }
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
    }
    if ((encoderPosRight < (targetDistanceRight + 5) && encoderPosRight > (targetDistanceRight - 5)) && Right_Done != true)
    {
      analogWrite(RIGHT_PWM_PIN, 0);
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      Right_Done = true;
    }
    else
    {
      int motorSpeedRight = pid_Forward_Right.compute(encoderPosRight);
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
      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    }
    digitalWrite(STANDBY_PIN, HIGH);
    delay(100);
    if (Left_Done == true and Right_Done == true)
    {
      Both_Done = true;
      encoderPosLeft -= targetDistanceLeft;
      encoderPosRight -= targetDistanceRight;
    }
    else
    {
      Right_Done = false;
      Left_Done = false;
    }
  }
  digitalWrite(STANDBY_PIN, LOW);
  delay(500);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
