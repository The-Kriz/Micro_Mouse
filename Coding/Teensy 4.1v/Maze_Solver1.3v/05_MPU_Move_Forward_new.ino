////////////////////////////////////////////////////////////////////////////////////////////////////////
void MPU_Move_Forward(int targetDistanceLeft = forwardTargetDistance, int targetDistanceRight = forwardTargetDistance, int Target_Angle = Read_MPU())
{
  pid_Forward_Left.setpoint(targetDistanceLeft);
  pid_Forward_Right.setpoint(targetDistanceRight);
  int angle_setPoint = Turn_Angle();
  PID_MPU.setpoint(0);
  int FWD_Initial_Speed = 40;
  int MPU_motorSpeedLeft =  FWD_Initial_Speed;
  int MPU_motorSpeedRight =  FWD_Initial_Speed;
  bool Both_Done = false;
  bool Right_Done = false;
  bool Left_Done = false;
  bool initial = false;
  digitalWrite(STANDBY_PIN, HIGH);
  while (!Both_Done)
  {
    while ((encoderPosRight < (targetDistanceRight - 10) or encoderPosLeft < (targetDistanceRight - 10)) and initial != true )
    {
      digitalWrite(RIGHT_IN1_PIN, HIGH);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      digitalWrite(LEFT_IN1_PIN, HIGH);
      digitalWrite(LEFT_IN2_PIN, LOW);
      double z = Read_MPU();
      z = z - angle_setPoint;

      bt.print("angle:");
      bt.println(z);
      int motorSpeed_MPU = PID_MPU.compute(z);
      if (z == 0)
      {
        MPU_motorSpeedLeft =  FWD_Initial_Speed;
        MPU_motorSpeedRight =  FWD_Initial_Speed;
      }
      else if (z < 0)
      {
        MPU_motorSpeedLeft =  FWD_Initial_Speed + abs(motorSpeed_MPU);
      }
      else
      {
        MPU_motorSpeedLeft =  FWD_Initial_Speed - abs(motorSpeed_MPU);
      }
      analogWrite(LEFT_PWM_PIN, MPU_motorSpeedLeft);
      analogWrite(RIGHT_PWM_PIN, MPU_motorSpeedRight);
    }
    initial = true;

    if ((encoderPosLeft < (targetDistanceLeft + 10) && encoderPosLeft > (targetDistanceLeft - 10)) and Left_Done != true)
    {
      analogWrite(LEFT_PWM_PIN, 0);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, LOW);
      Left_Done = true;
    }
    else if (Left_Done != true)
    {
      int motorSpeedLeft = pid_Forward_Left.compute(encoderPosLeft);
      if ( encoderPosLeft < targetDistanceLeft)
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
    if ((encoderPosRight < (targetDistanceRight + 10) && encoderPosRight > (targetDistanceRight - 10)) && Right_Done != true)
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
