///////////////////////////////////////////////////////////////////////////////////////////////
// TOF SENSOR PINS
int sensor_Lower_Limits[3] = {0, 0, 0}; // {100, 100, 100};
int sensor_Upper_Limits[3] = {220, 300, 220};
///////////////////////////////////////////////////////////////////////////////////////////////
// ENCODER VARIABLES
volatile int encoderPosLeft = 0;
volatile int encoderPosRight = 0;
volatile boolean aStateLeft;
volatile boolean bStateLeft;
volatile boolean aStateRight;
volatile boolean bStateRight;
///////////////////////////////////////////////////////////////////////////////////////////////
// PID_Forward_VARIABLES
#define Forward_LEFT_KP  1.4050    //0.650  // PID constants for Left Motor
#define Forward_LEFT_KD  0.0000   //0.100
#define Forward_LEFT_KI  0.0000
#define Forward_RIGHT_KP 1.4100   //0.680  // PID constants for Right Motor
#define Forward_RIGHT_KD 0.0000   //0.100
#define Forward_RIGHT_KI 0.0000
// PID_Rotation_VARIABLES
#define LEFT_KP  1.6000   //0.650  // PID constants for Left Motor
#define LEFT_KD  1.0000   //0.100
#define LEFT_KI  0.0000
#define RIGHT_KP 1.6000   //0.680  // PID constants for Right Motor
#define RIGHT_KD 1.0000   //0.100
#define RIGHT_KI 0.0000
///////////////////////////////////////////////////////////////////////////////////////////////
// CELL TRAVEL DISTANCE (ENCODER COUNT)
int Home_cell_forward = 410;
int forwardTargetDistanceLeft = 726;      //410  // 726 total cell distance // Target distance to move forward (in encoder counts)
int forwardTargetDistanceRight = 726;
int TurnForwardTargetDistance = 420;
int LeftrotationTargetDistanceLeft = 0;     // Target distance to rotate motors (in encoder counts)
int LeftrotationTargetDistanceRight = 530;  //550 rotation //29 per cm
int RightrotationTargetDistanceLeft = 530;    // Target distance to rotate motors (in encoder counts)
int RightrotationTargetDistanceRight = 0;
///////////////////////////////////////////////////////////////////////////////////////////////
// OTHER
bool startButtonPressed = false;
const int LED = 13;
