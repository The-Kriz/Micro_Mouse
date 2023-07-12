////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  bt.begin(9600); // BT-MODULE (HC05)
  pinMode(START_BUTTON, INPUT_PULLUP);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS );
  leds[0] = CRGB::Aqua;
  FastLED.show();

  pinMode(LEFT_PWM_PIN, OUTPUT);  // MOTOR DRIVER
  pinMode(LEFT_IN1_PIN, OUTPUT);
  pinMode(LEFT_IN2_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(RIGHT_IN1_PIN, OUTPUT);
  pinMode(RIGHT_IN2_PIN, OUTPUT);
  pinMode(STANDBY_PIN, OUTPUT);
  pinMode(LeftTOF, OUTPUT);  // TOF
  pinMode(CenterTOF, OUTPUT);
  pinMode(RightTOF, OUTPUT);
  digitalWrite(LeftTOF, LOW);
  digitalWrite(CenterTOF, LOW);
  digitalWrite(RightTOF, LOW);


  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN_A), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN_A), rightEncoderISR, CHANGE);

  pidLeft.begin();
  pidLeft.tune(LEFT_KP, LEFT_KI, LEFT_KD);
  pidLeft.limit(40, 240);   //(30, 255);
  pidRight.begin();
  pidRight.tune(RIGHT_KP, RIGHT_KI, RIGHT_KD);
  pidRight.limit(40, 240);  //(30, 255);

  pid_Forward_Left.begin();
  pid_Forward_Left.tune(Forward_LEFT_KP, Forward_LEFT_KI, Forward_LEFT_KD);
  pid_Forward_Left.limit(30, 255);   //(30, 255);
  pid_Forward_Right.begin();
  pid_Forward_Right.tune(Forward_RIGHT_KP, Forward_RIGHT_KI, Forward_RIGHT_KD);
  pid_Forward_Right.limit(30, 255);  //(30, 255);


  Wire2.begin();
  Serial.println("Assigning TOF Address");           // TOF ADDRESS SETUP
  bt.println("Assigning TOF Address");
  Serial.println("L");           // TOF ADDRESS SETUP
  bt.println("L");
  digitalWrite(LeftTOF, HIGH);
  delay(150);
  sensorL.setBus(&Wire2);
  sensorL.init(true);
  delay(100);
  sensorL.setAddress((uint8_t)1);
  Serial.println("C");
  bt.println("C");
  digitalWrite(CenterTOF, HIGH);
  delay(150);
  sensorC.setBus(&Wire2);
  sensorC.init(true);
  delay(100);
  sensorC.setAddress((uint8_t)2);
  Serial.println("R");
  bt.println("R");
  digitalWrite(RightTOF, HIGH);
  delay(150);
  sensorR.setBus(&Wire2);
  sensorR.init(true);
  delay(100);
  sensorR.setAddress((uint8_t)3);
  Serial.println("Done");
  bt.println("Done");
  sensorL.startContinuous();
  sensorC.startContinuous();
  sensorR.startContinuous();
  leds[0] = CRGB::Maroon;
  FastLED.show();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
