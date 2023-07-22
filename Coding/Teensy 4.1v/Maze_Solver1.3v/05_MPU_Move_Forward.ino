////////////////////////////////////////////////////////////////////////////////////////////////////////
void MPU_Move_Forward(int targetDistanceLeft = forwardTargetDistance, int targetDistanceRight = forwardTargetDistance, int Target_Angle = 0)
{
  pid_Forward_Left.setpoint(targetDistanceLeft);
  pid_Forward_Right.setpoint(targetDistanceRight);
  PID_MPU.setpoint(0);
  bool Both_Done = false;
  bool Right_Done = false;
  bool Left_Done = false;
  while (!Both_Done)
  {
    double z = Read_MPU();
    z = z - target_angle;
    Serial.print("angle:");
    Serial.println(z);
    bt.print("angle:");
    bt.println(z);
    int motorSpeed_MPU = PID_MPU.compute(z);

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
        if (motorSpeed_MPU > 0)
        {
          motorSpeedLeft =  motorSpeedLeft + motorSpeed_MPU;
        }
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
        if (motorSpeed_MPU < 0)
        {
          motorSpeedRight =  motorSpeedRight + motorSpeed_MPU;
        }
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
