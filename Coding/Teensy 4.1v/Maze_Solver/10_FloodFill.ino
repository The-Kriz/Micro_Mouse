////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NORTH 0   // Define constants for orientation
#define EAST 1
#define SOUTH 2
#define WEST 3
int destX = 8;
int destY = 8;
int MAZE_SIZE = 16;
////////////////////////////////////////////////////////////////////////////////////////////////////////
int currentOrientation = NORTH; // Global variable for current orientation

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
}mazes;

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
void flood_fill(struct maze *maze, int x, int y, int destX, int destY)   // Flood fill algorithm with destination in the center of the maze
{
  // Create a stack to store cell coordinates
  struct 
  {
    int x;
    int y;
  }stack[MAZE_SIZE * MAZE_SIZE];
  int top = 0; // Stack top index

  // Push the starting cell onto the stack
  stack[top].x = x;
  stack[top].y = y;
  maze->cells[x][y].visited = true;

  // Continue until the stack is empty
  while (top >= 0) {
    // Pop the top cell from the stack
    int currentX = stack[top].x;
    int currentY = stack[top].y;
    top--;

    // Check if the current cell is the destination
    if (currentX == destX && currentY == destY)
      return;

    //update walls for the current cell if needed
    update_walls(maze, currentX, currentY, Left_Wall(), Center_Wall(), Right_Wall());

    // Check neighboring cells
    if (currentX + 1 < MAZE_SIZE && !maze->cells[currentX + 1][currentY].visited && maze->cells[currentX + 1][currentY].centre == 0) {
      stack[++top].x = currentX + 1;
      stack[top].y = currentY;
      maze->cells[currentX + 1][currentY].visited = true;
      Turn_Right(); // Turn towards the next cell
      update_orientation_right();
    }
    if (currentX - 1 >= 0 && !maze->cells[currentX - 1][currentY].visited && maze->cells[currentX - 1][currentY].centre == 0)
    {
      stack[++top].x = currentX - 1;
      stack[top].y = currentY;
      maze->cells[currentX - 1][currentY].visited = true;
      Turn_Left(); // Turn towards the next cell
      update_orientation_left();
    }
    if (currentY + 1 < MAZE_SIZE && !maze->cells[currentX][currentY + 1].visited && maze->cells[currentX][currentY + 1].centre == 0)
    {
      stack[++top].x = currentX;
      stack[top].y = currentY + 1;
      maze->cells[currentX][currentY + 1].visited = true;
      // No need to turn as the bot is already oriented towards the next cell Move Forward
      Move_Forward();
      update_orientation_forward(currentOrientation);
    }
    if (currentY - 1 >= 0 && !maze->cells[currentX][currentY - 1].visited && maze->cells[currentX][currentY - 1].centre == 0)
    {
      stack[++top].x = currentX;
      stack[top].y = currentY - 1;
      maze->cells[currentX][currentY - 1].visited = true;
      U_Turn();
      update_orientation_right();
      update_orientation_right();
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////