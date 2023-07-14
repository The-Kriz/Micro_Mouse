#include <Queue.h> // Include the Arduino Queue library

class Location {
private:
  bool walls[4];
  int position[2];
  bool visited;

public:
  Location(int* pos = nullptr) {
    walls[0] = false;
    walls[1] = false;
    walls[2] = false;
    walls[3] = false;
    if (pos != nullptr) {
      position[0] = pos[0];
      position[1] = pos[1];
    } else {
      position[0] = -1;
      position[1] = -1;
    }
    visited = false;
  }

  void set_position(int pos[2]) {
    position[0] = pos[0];
    position[1] = pos[1];
  }
  void set_walls(bool walls[4]) {
    this->walls[0] = walls[0];
    this->walls[1] = walls[1];
    this->walls[2] = walls[2];
    this->walls[3] = walls[3];
  }
  void set_visited(bool vis) {
    visited = vis;
  }
  bool can_move_to(Location loc) {
    return (loc.position[0] == this->position[0] && loc.position[1] - this->position[1] == +1 && !this->walls[0]) ||
           (loc.position[1] == this->position[1] && loc.position[0] - this->position[0] == +1 && !this->walls[1]) ||
           (loc.position[0] == this->position[0] && loc.position[1] - this->position[1] == -1 && !this->walls[2]) ||
           (loc.position[1] == this->position[1] && loc.position[0] - this->position[0] == -1 && !this->walls[3]);
  }
};

class State {
private:
  static const int MAZE_WIDTH = 16;
  static const int MAZE_HEIGHT = 16;
  int location[2];
  State* parent;
  int action;
  int cur_dir;

public:
  State(int loc[2], State* parent = nullptr, int action = -1, int cur_dir = 0) {
    this->location[0] = loc[0];
    this->location[1] = loc[1];
    if (parent == nullptr) {
      this->parent = this;
    } else {
      this->parent = parent;
    }
    this->action = action;
    this->cur_dir = cur_dir;
  }

  void set_loc(int loc[2]) {
    this->location[0] = loc[0];
    this->location[1] = loc[1];
  }

  void set_par(State* par) {
    this->parent = par;
  }

  void set_act(int act) {
    this->action = act;
  }

  void set_cur_dir(int cur_dir) {
    this->cur_dir = cur_dir;
  }

  bool is_goal() {
    return (this->location[0] == 7 && this->location[1] == 7) || (this->location[0] == 7 && this->location[1] == 8) ||
           (this->location[0] == 8 && this->location[1] == 7) || (this->location[0] == 8 && this->location[1] == 8);
  }
};

const int State::MAZE_WIDTH;
const int State::MAZE_HEIGHT;
bool destination = false;

// for tracking global direction
// 0 = North
// 1 = East
// 2 = South
// 3 = West
int cur_direction = 0;

// for tracking global 'physical' position in maze as [x, y], initialized to [0, 0]
int cur_position[2] = {0, 0};

// for tracking all maze data, create 2d array of Locations
Location maze[State::MAZE_HEIGHT][State::MAZE_WIDTH];

// location object queue for tracking locations that may need to be explored during mapping
Queue<Location> loc_queue;

// direction stack for easy backtracking through maze when a dead end is found during mapping
Stack<int> dir_stack;

// action stack for processing optimal sequence of actions to find goal state
Stack<int> act_stack;

// state object queue for unexplored nodes during breadth first search
Queue<State> frontier;

// Function to check if there are walls around the current state
void get_walls(bool* walls) {
  walls[cur_direction] = true;  // Replace with appropriate code to check if there's a wall in front
  walls[(cur_direction + 1) % 4] = true;  // Replace with appropriate code to check if there's a wall to the right
  walls[(cur_direction + 2) % 4] = true;  // Replace with appropriate code to check if there's a wall behind
  walls[(cur_direction + 3) % 4] = true;  // Replace with appropriate code to check if there's a wall to the left
}

// Function to mark a given node as visited
void mark_visited_api(int* pos = nullptr) {
  if (pos == nullptr) {
    pos = cur_position;
  }
  // Replace with appropriate code to mark the node as visited in the API
}

// Function to mark a given node as part of the solution path
void mark_solution_api(int* pos = nullptr) {
  if (pos == nullptr) {
    pos = cur_position;
  }
  // Replace with appropriate code to mark the node as part of the solution path in the API
}

// Function to mark a given node as part of the BFS
void mark_bfs_api(int* pos = nullptr) {
  if (pos == nullptr) {
    pos = cur_position;
  }
  // Replace with appropriate code to mark the node as part of the BFS in the API
}

// Function to mark a node that has been used in the backtracking part of the algorithm
void mark_bktrk_api(int* pos = nullptr) {
  if (pos == nullptr) {
    pos = cur_position;
  }
  // Replace with appropriate code to mark the node as used in the backtracking in the API
}

// Function to print to the console
void log(String message) {
  Serial.println(message);
}

// Function to update the position (-1 is move backward, 1 is move forward)
void update_position(int move_direction = 1) {
  if (cur_direction == 0) {    // facing north
    cur_position[1] += move_direction;
  } else if (cur_direction == 1) {  // facing east
    cur_position[0] += move_direction;
  } else if (cur_direction == 2) {  // facing south
    cur_position[1] -= move_direction;
  } else if (cur_direction == 3) {  // facing west
    cur_position[0] -= move_direction;
  }
}

// Function to update the direction (-1 is left, 1 is right)
void update_direction(int turn_direction) {
  cur_direction = (cur_direction + turn_direction) % 4;
}

// Function to turn toward an adjacent location object
void turn_toward(Location* loc) {
  int new_dir = cur_direction;
  // find direction of adjacent location
  if (cur_position[0] == loc->position[0]) {  // if two locations have the same x coordinate
    if (cur_position[1] - loc->position[1] == 1) {  // if I am above next location, turn south
      new_dir = 2;
    } else {  // otherwise, I must be below the next location
      new_dir = 0;
    }
  } else {  // two directions have the same y coordinate
    if (cur_position[0] - loc->position[0] == 1) {  // if I am to the right of location, turn west
      new_dir = 3;
    } else {  // I must be to the left of the location
      new_dir = 1;
    }
  }
  set_dir(new_dir);
}

// Function to move forward
void move_forward() {
  // Replace with appropriate code to move the robot forward
  update_position(1);  // update current position
}

// Function to turn left
void turn_left() {
  // Replace with appropriate code to turn the robot left
  update_direction(-1);  // turn left
}

// Function to turn right
void turn_right() {
  // Replace with appropriate code to turn the robot right
  update_direction(1);  // turn right
}

// Function to turn around
void turn_around() {
  turn_right();
  turn_right();
}

// Function to set direction to a specific direction
void set_dir(int new_dir) {
  if (new_dir == cur_direction) {  // if already facing the correct direction
    return;
  }
  if (new_dir == (cur_direction + 1) % 4) {  // if need to turn right once
    turn_right();
    return;
  }
  if (new_dir == (cur_direction + 2) % 4) {  // if need to turn around
    turn_right();
    turn_right();
    return;
  }
  turn_left();  // if need to turn left once
}

// Function to map the maze in depth-first search using loc_queue
void dfs_map_maze() {
  Location* cur_loc = &maze[cur_position[0]][cur_position[1]];

  if (!cur_loc->visited) {  // if the current location has not been visited
    cur_loc->visited = true;  // mark the location as visited
    get_walls(cur_loc->walls);  // set wall locations
    mark_visited_api(cur_position);  // mark the current position in the API

    // if I have no north wall and the north location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[0] && !maze[cur_position[0]][cur_position[1] + 1].visited) {
      loc_queue.enqueue(&maze[cur_position[0]][cur_position[1] + 1]);
    }

    // if I have no east wall and the east location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[1] && !maze[cur_position[0] + 1][cur_position[1]].visited) {
      loc_queue.enqueue(&maze[cur_position[0] + 1][cur_position[1]]);
    }

    // if I have no south wall and the south location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[2] && !maze[cur_position[0]][cur_position[1] - 1].visited) {
      loc_queue.enqueue(&maze[cur_position[0]][cur_position[1] - 1]);
    }

    // if I have no west wall and the west location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[3] && !maze[cur_position[0] - 1][cur_position[1]].visited) {
      loc_queue.enqueue(&maze[cur_position[0] - 1][cur_position[1]]);
    }
  }

  while (true) {
    if (cur_position[0] == State::MAZE_WIDTH / 2 && cur_position[1] == State::MAZE_HEIGHT / 2) {
      destination = true;
    }
    if (destination) {
      if (!(cur_position[0] == 0 && cur_position[1] == 0)) {
        set_dir((dir_stack.pop() + 2) % 4);  // turn around
        move_forward();
        dfs_map_maze();  // try to move again
      }
      if (cur_position[0] == 0 && cur_position[1] == 0) {
        return;  // if we are back at the initial position, we are done
      }
    }
    if (loc_queue.isEmpty()) {  // if loc_queue is empty, backtrack to the initial position then return
      if (!(cur_position[0] == 0 && cur_position[1] == 0)) {
        set_dir((dir_stack.pop() + 2) % 4);  // turn around
        move_forward();
        dfs_map_maze();  // try to move again
      }
      return;
    }

    Location* next_loc = loc_queue.dequeue();  // otherwise, take locations off of the loc_queue until we get an unvisited one
    if (!next_loc->visited) {
      break;
    }
  }

  // if I can move to that location from where I am, turn toward the new location, save that direction, and move forward
  if (cur_loc->can_move_to(*next_loc)) {
    turn_toward(next_loc);
    dir_stack.push(cur_direction);  // save the current direction for backtracking on the direction stack
    move_forward();
  } else {   // put the target location back on the loc_queue, back up one square, then try again
    loc_queue.enqueue(next_loc);
    set_dir((dir_stack.pop() + 2) % 4);  // turn toward the last position
    move_forward();
  }
  dfs_map_maze();  // try to move again
}

// Function to find the shortest path using breadth-first search
State* find_bfs_shortest_path() {
  // initialize all locations to unvisited
  for (int i = 0; i < State::MAZE_HEIGHT; i++) {
    for (int j = 0; j < State::MAZE_WIDTH; j++) {
      maze[i][j].visited = false;
    }
  }

  Location first_loc = maze[0][0];  // generate the initial location
  State* first_state = new State(first_loc);  // generate the initial state: parent is self, action is none
  mark_bfs_api(first_state->location);  // mark the initial state in the API
  frontier.enqueue(*first_state);  // add the initial state to the frontier queue

  while (!frontier.isEmpty()) {
    State cur_state = frontier.dequeue();  // take the current state off of the frontier queue

    if (cur_state.is_goal()) {  // if the current state is the goal state, we are done
      return &cur_state;
    }

    Location* cur_loc = &maze[cur_state.location[0]][cur_state.location[1]];
    if (!cur_loc->visited) {
      cur_loc->visited = true;  // mark the location as visited

      // if I have no north wall and the north location is not visited, enqueue a new state to explore later
      if (!cur_loc->walls[0] && !maze[cur_state.location[0]][cur_state.location[1] + 1].visited) {
        State next_state(maze[cur_state.location[0]][cur_state.location[1] + 1]);
        next_state.parent = &cur_state;
        next_state.action = 0;  // move forward
        next_state.cur_dir = cur_direction;
        frontier.enqueue(next_state);
      }

      // if I have no east wall and the east location is not visited, enqueue a new state to explore later
      if (!cur_loc->walls[1] && !maze[cur_state.location[0] + 1][cur_state.location[1]].visited) {
        State next_state(maze[cur_state.location[0] + 1][cur_state.location[1]]);
        next_state.parent = &cur_state;
        next_state.action = 0;  // move forward
        next_state.cur_dir = cur_direction;
        frontier.enqueue(next_state);
      }

      // if I have no south wall and the south location is not visited, enqueue a new state to explore later
      if (!cur_loc->walls[2] && !maze[cur_state.location[0]][cur_state.location[1] - 1].visited) {
        State next_state(maze[cur_state.location[0]][cur_state.location[1] - 1]);
        next_state.parent = &cur_state;
        next_state.action = 0;  // move forward
        next_state.cur_dir = cur_direction;
        frontier.enqueue(next_state);
      }

      // if I have no west wall and the west location is not visited, enqueue a new state to explore later
      if (!cur_loc->walls[3] && !maze[cur_state.location[0] - 1][cur_state.location[1]].visited) {
        State next_state(maze[cur_state.location[0] - 1][cur_state.location[1]]);
        next_state.parent = &cur_state;
        next_state.action = 0;  // move forward
        next_state.cur_dir = cur_direction;
        frontier.enqueue(next_state);
      }
    }
  }

  return nullptr;  // return null if no solution is found
}

// Function to generate the optimal sequence of actions based on the shortest path
void generate_actions(State* goal_state) {
  State* cur_state = goal_state;

  while (cur_state->parent != cur_state) {  // while the current state is not the initial state
    act_stack.push(cur_state->action);  // add the current action to the action stack
    cur_state = cur_state->parent;  // move to the parent state
  }
}

void setup() {
  // Replace with appropriate code to initialize any necessary components or libraries

  // Replace with appropriate code to initialize the Serial communication
  Serial.begin(9600);

  // Initialize maze with default values
  for (int i = 0; i < State::MAZE_HEIGHT; i++) {
    for (int j = 0; j < State::MAZE_WIDTH; j++) {
      int pos[2] = {i, j};
      maze[i][j].set_position(pos);
    }
  }
}

void loop() {
  // Mapping phase
  dfs_map_maze();

  // Finding the shortest path phase
  State* goal_state = find_bfs_shortest_path();
  if (goal_state != nullptr) {
    generate_actions(goal_state);
  }

  // Execution phase
  while (!act_stack.isEmpty()) {
    int action = act_stack.pop();
    // Perform the action based on the value
    if (action == 0) {
      move_forward();
    } else if (action == 1) {
      turn_left();
    } else if (action == 2) {
      turn_right();
    }
  }

  // Mark the solution path in the API
  mark_solution_api();

  // End of the algorithm
  // Replace with appropriate code to perform any necessary cleanup or stop the robot
}
