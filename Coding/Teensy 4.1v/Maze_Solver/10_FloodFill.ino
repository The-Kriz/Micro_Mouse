////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NORTH 0   // Define constants for orientation
#define EAST 1
#define SOUTH 2
#define WEST 3
////////////////////////////////////////////////////////////////////////////////////////////////////////
int currentOrientation = NORTH; // Global variable for current orientation
int Glob_destX = 8;
int Glob_destY = 8;
int Glob_startX = 0;
int Glob_startY = 14;
int MAZE_SIZE = 16;
int Direction = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////
int orient = 0;              // Initial orientation
struct cell                 // Define the cell struct
{
  int x;
  int y;
  int left;
  int right;
  int centre;
  bool visited;
};
struct maze                  // Define the maze struct
{
  struct cell cells[16][16];
} mazes;

////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_maze(struct maze *mazes) {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      mazes->cells[i][j].x = i;
      mazes->cells[i][j].y = j;
      mazes->cells[i][j].left = 0;
      mazes->cells[i][j].right = 0;
      mazes->cells[i][j].centre = 0;
      mazes->cells[i][j].visited = false;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_walls(struct maze *maze, int x, int y, int left, int right, int centre)  // Function to update Set walls
{
  maze->cells[x][y].left = left;
  maze->cells[x][y].right = right;
  maze->cells[x][y].centre = centre;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_orientation_left()                       // Function to update the orientation after a left turn
{
  currentOrientation = (currentOrientation - 1 + 4) % 4;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_orientation_right()                      // Function to update the orientation after a right turn
{
  currentOrientation = (currentOrientation + 1) % 4;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_orientation_forward(int moveOrientation) // Function to update the orientation after moving forward
{
  currentOrientation = moveOrientation;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_walls_based_on_orientation(struct maze* maze, int x, int y, int Direction, int left_wall, int right_wall, int centre_wall) {
  switch (currentOrientation) {
    case NORTH:
      if (Direction == 0)
        update_walls(maze, x, y - 1, left_wall, right_wall, centre_wall);
      else if (Direction == 2)
        update_walls(maze, x - 1, y, left_wall, right_wall, centre_wall);
      else if (Direction == 1)
        update_walls(maze, x + 1, y, left_wall, right_wall, centre_wall);
      break;
    case EAST:
      if (Direction == 0)
        update_walls(maze, x + 1, y, left_wall, right_wall, centre_wall);
      else if (Direction == 2)
        update_walls(maze, x, y - 1, left_wall, right_wall, centre_wall);
      else if (Direction == 1)
        update_walls(maze, x, y + 1, left_wall, right_wall, centre_wall);
      break;
    case SOUTH:
      if (Direction == 0)
        update_walls(maze, x, y + 1, left_wall, right_wall, centre_wall);
      else if (Direction == 2)
        update_walls(maze, x + 1, y, left_wall, right_wall, centre_wall);
      else if (Direction == 1)
        update_walls(maze, x - 1, y, left_wall, right_wall, centre_wall);
      break;
    case WEST:
      if (Direction == 0)
        update_walls(maze, x - 1, y, left_wall, right_wall, centre_wall);
      else if (Direction == 2)
        update_walls(maze, x, y + 1, left_wall, right_wall, centre_wall);
      else if (Direction == 1)
        update_walls(maze, x, y - 1, left_wall, right_wall, centre_wall);
      break;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_maze(struct maze *maze)
{
  for (int i = 0; i < MAZE_SIZE; i++)
  {
    for (int j = 0; j < MAZE_SIZE; j++)
    {
      if (maze->cells[i][j].centre == 0)
      {
        Serial.print(". ");
      }
      else
      {
        Serial.print("# ");
      }
    }
    Serial.println("");
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void flood_fill(struct maze *maze, int x, int y, int destX, int destY)   // Flood fill algorithm with destination in the center of the maze
{
  // Create a stack to store cell coordinates
  struct
  {
    int x;
    int y;
  } stack[MAZE_SIZE * MAZE_SIZE];
  int top = 0; // Stack top index

  // Push the starting cell onto the stack
  stack[top].x = x;
  stack[top].y = y;
  maze->cells[x][y].visited = true;

  // Continue until the stack is empty
  while (top >= 0)
  {
    Serial.println("start of loop");
    bt.println("start of loop");

    // Pop the top cell from the stack
    int currentX = stack[top].x;
    int currentY = stack[top].y;
    top--;

    // Check if the current cell is the destination
    if (currentX == destX && currentY == destY)
      return;

    //update walls for the current cell if needed
    update_walls_based_on_orientation(maze, currentX, currentY, Direction, Left_Wall(), Center_Wall(), Right_Wall());
    leds[0] = CRGB::Black;
    FastLED.show();

    // Check neighboring cells
    if (currentX + 1 < MAZE_SIZE && !maze->cells[currentX + 1][currentY].visited && maze->cells[currentX][currentY].right == 0)
    {
      stack[++top].x = currentX + 1;
      stack[top].y = currentY;
      maze->cells[currentX + 1][currentY].visited = true;
      Serial.println("Turn Right Request");
      bt.println("Turn Right Request");
      Turn_Right(); // Turn towards the next cell
      Serial.println("Turn Right Request completed");
      bt.println("Turn Right Request completed");
      update_orientation_right();
      Serial.print("Current Orientation :");
      Serial.println(currentOrientation);
      bt.print("Current Orientation :");
      bt.println(currentOrientation);
      print_maze(maze);
      Direction = 1;

    }
    else if (currentX - 1 >= 0 && !maze->cells[currentX - 1][currentY].visited && maze->cells[currentX][currentY].left == 0)
    {
      stack[++top].x = currentX - 1;
      stack[top].y = currentY;
      maze->cells[currentX - 1][currentY].visited = true;
      Serial.println("Turn Left Request");
      bt.println("Turn Left Request");
      Turn_Left(); // Turn towards the next cell
      Serial.println("Turn Left Request completed");
      bt.println("Turn Left Request completed");
      update_orientation_left();
      Serial.print("Current Orientation :");
      Serial.println(currentOrientation);
      bt.print("Current Orientation :");
      bt.println(currentOrientation);
      print_maze(maze);
      Direction = 2;
    }
    else if (currentY + 1 < MAZE_SIZE && !maze->cells[currentX][currentY + 1].visited && maze->cells[currentX][currentY].centre == 0)
    {
      stack[++top].x = currentX;
      stack[top].y = currentY + 1;
      maze->cells[currentX][currentY + 1].visited = true;
      // No need to turn as the bot is already oriented towards the next cell Move Forward
      Serial.println("Move Forward Request");
      bt.println("Move Forward Request");
      Move_Forward();
      Serial.println("Move Forward Request completed");
      bt.println("Move Forward Request completed");
      update_orientation_forward(currentOrientation);
      Serial.print("Current Orientation :");
      Serial.println(currentOrientation);
      bt.print("Current Orientation :");
      bt.println(currentOrientation);
      print_maze(maze);
      Direction = 0;
    }
    else if (currentY - 1 >= 0 && !maze->cells[currentX][currentY - 1].visited && !maze->cells[currentX][currentY].centre == 0 && !maze->cells[currentX][currentY].left == 0 && !maze->cells[currentX][currentY].left == 0)
    {
      stack[++top].x = currentX;
      stack[top].y = currentY - 1;
      maze->cells[currentX][currentY - 1].visited = true;
      Serial.println("U Turn Request");
      bt.println("U Turn Request");
      U_Turn();
      Serial.println("U Turn Request completed");
      bt.println("U Turn Request completed");
      update_orientation_right();
      update_orientation_right();
      Serial.print("Current Orientation :");
      Serial.println(currentOrientation);
      bt.print("Current Orientation :");
      bt.println(currentOrientation);
      print_maze(maze);
    }
    Serial.println("end of loop");
    bt.println("end of loop");

    Pause(maze);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
