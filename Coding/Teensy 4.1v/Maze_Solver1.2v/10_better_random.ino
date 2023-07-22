//North: 0
//East: 1
//South: 2
//West: 3
const int MAZE_WIDTH = 16;
const int MAZE_HEIGHT = 16;

// Define the maze and initialize all cells to unvisited (0)
int maze[MAZE_WIDTH][MAZE_HEIGHT] = {0};
int current_x = 0;
int current_y = 15;
int current_orientation = 0;
// Function to check if a cell has been visited
bool isCellVisited(int x, int y) {
  return maze[x][y] == 1;
}

// Function to mark a cell as visited
void markCellAsVisited(int x, int y) {
  maze[x][y] = 1;
}
void updateOrientation(bool turn_right) {
  if (turn_right) {
    current_orientation = (current_orientation + 1) % 4; // Rotate clockwise
  } else {
    current_orientation = (current_orientation + 3) % 4; // Rotate counterclockwise
  }
}
void makeDecision1()
{
  delay(200);
  int Wall_On_Left = Left_Wall();
  int Wall_On_Center = Center_Wall();
  int Wall_On_Right = Right_Wall();

  // Print wall information using Serial
  Serial.print("Wall on Left: ");
  Serial.println(Wall_On_Left);
  Serial.print("Wall on Center: ");
  Serial.println(Wall_On_Center);
  Serial.print("Wall on Right: ");
  Serial.println(Wall_On_Right);

  // Print wall information using Bluetooth
  bt.print("Wall on Left: ");
  bt.println(Wall_On_Left);
  bt.print("Wall on Center: ");
  bt.println(Wall_On_Center);
  bt.print("Wall on Right: ");
  bt.println(Wall_On_Right);

  // Initialize random seed
  randomSeed(analogRead(A0));

  if (Wall_On_Left && Wall_On_Center && Wall_On_Right)
  {
    Serial.println("U-Turn");
    bt.println("U-Turn");
    U_Turn();
    updateOrientation(true)
    updateOrientation(true)
  }
  else if (Wall_On_Left && Wall_On_Right)
  {
    Serial.println("Move Forward");
    bt.println("Move Forward");
    Move_Forward();
  }
  else if (Wall_On_Left && Wall_On_Center)
  {
    Serial.println("Turn Right");
    bt.println("Turn Right");
    Turn_Right();
  }
  else if (Wall_On_Right && Wall_On_Center)
  {
    Serial.println("Turn Left");
    bt.println("Turn Left");
    Turn_Left();
  }
  else if (Wall_On_Left)
  {
    Serial.println("Random Decision - Right or Forward");
    bt.println("Random Decision - Right or Forward");
    if (random(2) == 0)
    {
      Serial.println("Move Forward");
      bt.println("Move Forward");
      Move_Forward();
    }
    else
    {
      Serial.println("Turn Right");
      bt.println("Turn Right");
      Turn_Right();
    }
  }
  else if (Wall_On_Right)
  {
    Serial.println("Random Decision - Left or Forward");
    bt.println("Random Decision - Left or Forward");
    if (random(2) == 0)
    {
      Serial.println("Move Forward");
      bt.println("Move Forward");
      Move_Forward();
    }
    else
    {
      Serial.println("Turn Left");
      bt.println("Turn Left");
      Turn_Left();
    }
  }
  else if (Wall_On_Center)
  {
    Serial.println("Random Decision - Left or Right");
    bt.println("Random Decision - Left or Right");
    if (random(2) == 0)
    {
      Serial.println("Turn Right");
      bt.println("Turn Right");
      Turn_Right();
    }
    else
    {
      Serial.println("Turn Left");
      bt.println("Turn Left");
      Turn_Left();
    }
  }
  else
  {
    Serial.println("No Wall - Delay 2000ms");
    bt.println("No Wall");
    delay(1000);
  }
}
