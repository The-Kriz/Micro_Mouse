///////////////////////////////////////////////////////////////////////////////////////////////
// TOF SENSOR PINS
int sensor_Lower_Limits[3] = {0, 0, 0}; // {100, 100, 100};
int sensor_Upper_Limits[3] = {300, 450, 300};//{220, 300, 220};
///////////////////////////////////////////////////////////////////////////////////////////////
//  ENCODER VARIABLES
volatile int encoderPosLeft = 0;
volatile int encoderPosRight = 0;
volatile boolean aStateLeft;
volatile boolean bStateLeft;
volatile boolean aStateRight;
volatile boolean bStateRight;
///////////////////////////////////////////////////////////////////////////////////////////////
//  PID Speed Limit (PWM)
int Initial_Speed = 50;
int Rotation_LEFT_PID_Limits[2] = {30, 100};
int Rotation_RIGHT_PID_Limit[2] = {30, 100};
int Forward_LEFT_PID_Limits[2] = {30, 100};
int Forward_RIGHT_PID_Limit[2] = {30, 100};
///////////////////////////////////////////////////////////////////////////////////////////////
//  PID_MPU_Forward_VARIABLES
#define Forward_MPU_KP  80.0000
#define Forward_MPU_KD  0.0000
#define Forward_MPU_KI  0.0000
///////////////////////////////////////////////////////////////////////////////////////////////
//  PID_Forward_VARIABLES
#define Forward_LEFT_KP  1.3800    //0.650  // PID constants for Left Motor
#define Forward_LEFT_KD  1.8000   //0.100
#define Forward_LEFT_KI  0.0000

#define Forward_RIGHT_KP 1.3800   //0.680  // PID constants for Right Motor
#define Forward_RIGHT_KD 1.8000   //1.5000  
#define Forward_RIGHT_KI 0.0000
///////////////////////////////////////////////////////////////////////////////////////////////
// PID_Rotation_VARIABLES
#define Rotation_LEFT_KP  1.9000   //0.650  // PID constants for Left Motor
#define Rotation_LEFT_KD  1.1000  // 2.00 //0.100
#define Rotation_LEFT_KI  0.0000

#define Rotation_RIGHT_KP 1.9000   //0.680  // PID constants for Right Motor
#define Rotation_RIGHT_KD 1.1000   // 2.00  //0.100
#define Rotation_RIGHT_KI 0.0000
///////////////////////////////////////////////////////////////////////////////////////////////
//  TRAVEL DISTANCE (ENCODER COUNT)
int Home_cell_forward                = 400;       // Distance From Cell Wall To Next Cell Start
int forwardTargetDistance            = 750;        // Distance From Cell Start To Next Cell Start
int TurnForwardTargetDistance        = 620;       // Distance From Cell Start To Turn Location (cell Center)
int After_TurnForwardTargetDistance  = 160;       // Distance From Cell Center to Next Cell After Turn
int LeftrotationTargetDistanceLeft   = -270;      // Distance To Rotate Left Motor When Turning Left
int LeftrotationTargetDistanceRight  =  270;      // Distance To Rotate Right Motor When Turning Left
int RightrotationTargetDistanceLeft  =  270;      // Distance To Rotate Left Motor When Turning Right
int RightrotationTargetDistanceRight = -270;      // Distance To Rotate Right Motor When Turning Right
int U_TurnForwardTargetDistance      = 620;       // Distance From Cell Start To U Turn Location
///////////////////////////////////////////////////////////////////////////////////////////////
//  OTHER
bool startButtonPressed = false;
const int LED = 13;
const int BRIGHTNESS = 10;
int Target_Angle;
////////////////////////////////////////////////////////////////////////////////////////////////////////
