void Move_Forward(int targetDistanceLeft = forwardTargetDistanceLeft, int targetDistanceRight = forwardTargetDistanceRight)
{
  pid_Forward_Left.setpoint(targetDistanceLeft);
  pid_Forward_Right.setpoint(targetDistanceRight);
  bool Both_Done = false;
  bool Right_Done = false;
  bool Left_Done = false;
  digitalWrite(STANDBY_PIN, HIGH);
  while (!Both_Done)
  {
    int motorSpeedLeft = pid_Forward_Left.compute(encoderPosLeft);
    int motorSpeedRight = pid_Forward_Right.compute(encoderPosRight);
    if (encoderPosLeft < targetDistanceLeft)
    {
      digitalWrite(LEFT_IN1_PIN, HIGH);
      digitalWrite(LEFT_IN2_PIN, LOW);
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
    }
    else if (encoderPosLeft > (targetDistanceLeft + 5))
    {
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, HIGH);
      analogWrite(LEFT_PWM_PIN, abs(motorSpeedLeft));
    }
    else
    {
      analogWrite(LEFT_PWM_PIN, 0);
      digitalWrite(LEFT_IN1_PIN, LOW);
      digitalWrite(LEFT_IN2_PIN, LOW);
      Left_Done = true;
    }
    if (encoderPosRight < targetDistanceRight)
    {
      digitalWrite(RIGHT_IN1_PIN, HIGH);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));

    }
    else if (encoderPosRight > (targetDistanceRight + 5))
    {
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, HIGH);
      analogWrite(RIGHT_PWM_PIN, abs(motorSpeedRight));
    }
    else
    {
      analogWrite(RIGHT_PWM_PIN, 0);
      digitalWrite(RIGHT_IN1_PIN, LOW);
      digitalWrite(RIGHT_IN2_PIN, LOW);
      Right_Done = true;
    }
    Serial.print("Left Speed: ");  // BT MODULE (RX3 TX3)
    Serial.print(motorSpeedLeft);
    Serial.print(" Left Count: ");
    Serial.print(encoderPosLeft);
    bt.print("Left Speed: ");
    bt.print(motorSpeedLeft);
    bt.print(" Left Count: ");
    bt.print(encoderPosLeft);

    Serial.print(" Right Speed: ");
    Serial.print(motorSpeedRight);
    Serial.print(" Right Count: ");
    Serial.println(encoderPosRight);
    bt.print(" Right Speed: ");
    bt.print(motorSpeedRight);
    bt.print(" Right Count: ");
    bt.print(encoderPosRight);
    bt.println(" ");
    Serial.println(" ");
    if (Left_Done == true and Right_Done == true)
    {
      Both_Done = true;
      encoderPosLeft -= targetDistanceLeft;
      encoderPosRight -= targetDistanceRight;
      digitalWrite(STANDBY_PIN, LOW);
    }
  }
}
