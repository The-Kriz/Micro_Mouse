//#include <Queue.h> // Include the Arduino Queue library

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
      return (loc.position[0] == this->position[0] && loc.position[1] - this->position[1] == +1 && !this->walls[0])
             || (loc.position[1] == this->position[1] && loc.position[0] - this->position[0] == +1 && !this->walls[1])
             || (loc.position[0] == this->position[0] && loc.position[1] - this->position[1] == -1 && !this->walls[2])
             || (loc.position[1] == this->position[1] && loc.position[0] - this->position[0] == -1 && !this->walls[3]);
    }
};
class State {
    // loc is the physical location this state occupies taken as a list of [x, y] coordinates (ints)
    // parent is the adjacent state that generated this state (a State ref)
    // action is the action the parent took to reach this state, encoded as the 'turn' taken before moving forward
    //   turn can be 0 - no turn, 1 - turn right, 2 - turn around, 3 - turn left, or -1 if null action (see default)
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
      if (action == -1) {
        this->action = -1;
      } else {
        this->action = action;
      }
      if (cur_dir == 0) {
        this->cur_dir = 0;
      } else {
        this->cur_dir = cur_dir;
      }
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
      return (this->location[0] == 7 && this->location[1] == 7) || (this->location[0] == 7 && this->location[1] == 8)
             || (this->location[0] == 8 && this->location[1] == 7) || (this->location[0] == 8 && this->location[1] == 8)
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
Location maze[MAZE_HEIGHT][MAZE_WIDTH];

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
  walls[cur_direction] = API.wallFront();  // is there a wall in front
  walls[(cur_direction + 1) % 4] = API.wallRight();  // is there a wall to the right
  walls[(cur_direction + 2) % 4] = false;  // no wall from direction we came from
  walls[(cur_direction + 3) % 4] = API.wallLeft();  // is there a wall to the left
  if (cur_position[0] == 0 && cur_position[1] == 0) {  // if first square, mark bottom wall as there
    walls[2] = true;
  }
}

// Function to mark a given node as visited
void mark_visited_api(int* pos = NULL) {
  if (pos == NULL) {
    pos = cur_position;
  }
  API.setColor(pos[0], pos[1], "G");
  API.setText(pos[0], pos[1], "hit");
}

// Function to mark a given node as part of the solution path
void mark_solution_api(int* pos = NULL) {
  if (pos == NULL) {
    pos = cur_position;
  }
  API.setColor(pos[0], pos[1], "B");
  API.setText(pos[0], pos[1], "Sol");
}

// Function to mark a given node as part of the BFS
void mark_bfs_api(int* pos = NULL) {
  if (pos == NULL) {
    pos = cur_position;
  }
  API.setColor(pos[0], pos[1], "c");
  API.setText(pos[0], pos[1], "dfs");
}

// Function to mark a node that has been used in the backtracking part of the algorithm
void mark_bktrk_api(int* pos = NULL) {
  if (pos == NULL) {
    pos = cur_position;
  }
  API.setColor(pos[0], pos[1], "o");
  API.setText(pos[0], pos[1], "bktrk");
}

// Function to print to the console
void log(String message) {
  Serial.println(message);
}

// Function to update the position (-1 is move backward, 1 is move forward)
void update_position(int move_direction = 1) {
  if (cur_direction == 0) {    // facing north
    cur_position[1] += move_direction;
  }
  else if (cur_direction == 1) {  // facing east
    cur_position[0] += move_direction;
  }
  else if (cur_direction == 2) {  // facing south
    cur_position[1] -= move_direction;
  }
  else if (cur_direction == 3) {  // facing west
    cur_position[0] -= move_direction;
  }
}

// Function to update the direction (-1 is left, 1 is right)
void update_direction(int turn_direction) {
  cur_direction = (cur_direction + turn_direction) % 4;
}

// Function to turn toward an adjacent location object
void turn_toward(Location *loc) {
  int new_dir = cur_direction;
  // find direction of adjacent location
  if (cur_position[0] == loc.position[0]) {  // if two locations have the same x coordinate
    if (cur_position[1] - loc.position[1] == 1) {  // if i am above next location, turn south
      new_dir = 2;
    }
    else {  // otherwise i must be below next location
      new_dir = 0;
    }
  }
  else {  // two directions have the same y coordinate
    if (cur_position[0] - loc.position[0] == 1) {  // if i am to the right of location, turn west
      new_dir = 3;
    }
    else {  // i must be to the left of the location
      new_dir = 1;
    }
  }
  set_dir(new_dir);
}

// Function to move forward
void move_forward() {
  API.moveForward();  // move forward in maze
  update_position(1);  // update current position
}

// Function to turn left
void turn_left() {
  API.turnLeft();
  update_direction(-1);  // turn left
}

// Function to turn right
void turn_right() {
  API.turnRight();
  update_direction(1);  // turn right
}

// Function to turn around
void turn_around() {
  turn_right();
  turn_right();
}

// Function to set direction to a specific direction
void set_dir(int new_dir) {
  if (new_dir == cur_direction) {  // if already facing correct direction
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

  if (!cur_loc->visited) {  // if current location has not been visited
    cur_loc->visited = true;  // mark location as visited
    get_walls(cur_loc->walls);  // set wall locations
    mark_visited_api(cur_position);  // mark current position in API

    // if I have no north wall and north location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[0] && !maze[cur_position[0]][cur_position[1] + 1].visited) {
      loc_queue.enqueue(&maze[cur_position[0]][cur_position[1] + 1]);
    }

    // if I have no east wall and east location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[1] && !maze[cur_position[0] + 1][cur_position[1]].visited) {
      loc_queue.enqueue(&maze[cur_position[0] + 1][cur_position[1]]);
    }

    // if I have no south wall and south location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[2] && !maze[cur_position[0]][cur_position[1] - 1].visited) {
      loc_queue.enqueue(&maze[cur_position[0]][cur_position[1] - 1]);
    }

    // if I have no west wall and west location is not visited, put it on loc_queue to explore later
    if (!cur_loc->walls[3] && !maze[cur_position[0] - 1][cur_position[1]].visited) {
      loc_queue.enqueue(&maze[cur_position[0] - 1][cur_position[1]]);
    }
  }

  while (true) {
    if (cur_position[0] == MAZE_WIDTH / 2 && cur_position[1] == MAZE_HEIGHT / 2) {
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
    if (loc_queue.isEmpty()) {  // if loc_queue is empty, backtrack to initial position then return
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

  // if I can move to that location from where I am, turn toward new location, save that direction, and move forward
  if (cur_loc->can_move_to(*next_loc)) {
    turn_toward(*next_loc);
    dir_stack.push(cur_direction);  // save current direction for backtracking on the direction stack
    move_forward();
  }
  else {   // put the target location back on the loc_queue, back up one square, then try again
    loc_queue.enqueue(next_loc);
    set_dir((dir_stack.pop() + 2) % 4);  // turn toward last position
    move_forward();
  }
  dfs_map_maze();  // try to move again
}

// Function to find the shortest path using breadth-first search
State* find_bfs_shortest_path() {
  // initialize all locations to unvisited
  for (int i = 0; i < MAZE_HEIGHT; i++) {
    for (int j = 0; j < MAZE_WIDTH; j++) {
      maze[i][j].visited = false;
    }
  }

  Location first_loc = maze[0][0];  // generate initial location
  State* first_state = new State(first_loc);  // generate initial state: parent is self, action is null
  frontier.enqueue(first_state);  // enqueue first state

  while (!frontier.isEmpty()) {
    State* next_state = frontier.dequeue();  // dequeue next state
    maze[next_state->location.position[0]][next_state->location.position[1]].visited = true;  // mark location as visited
    mark_bfs_api(next_state->location.position);  // mark it on the API

    if (next_state->is_goal()) {
      return next_state;  // if it is the goal, return it
    }

    Location my_loc = next_state->location;

    if (!my_loc.walls[0] && my_loc.position[1] + 1 < MAZE_WIDTH) {
      Location north_loc = maze[my_loc.position[0]][my_loc.position[1] + 1];
    }
    if (!my_loc.walls[1] && my_loc.position[0] + 1 < MAZE_HEIGHT) {
      Location east_loc = maze[my_loc.position[0] + 1][my_loc.position[1]];
    }
    if (!my_loc.walls[2] && my_loc.position[1] - 1 >= 0) {
      Location south_loc = maze[my_loc.position[0]][my_loc.position[1] - 1];
    }
    if (!my_loc.walls[3] && my_loc.position[0] - 1 >= 0) {
      Location west_loc = maze[my_loc.position[0] - 1][my_loc.position[1]];
    }

    if (!my_loc.walls[0] && my_loc.can_move_to(north_loc) && !north_loc.visited) {
      State* north_state = new State(north_loc, next_state, (0 - next_state->cur_dir) % 4, 0);
      frontier.enqueue(north_state);
    }

    if (!my_loc.walls[1] && my_loc.can_move_to(east_loc) && !east_loc.visited) {
      State* east_state = new State(east_loc, next_state, (1 - next_state->cur_dir) % 4, 1);
      frontier.enqueue(east_state);
    }

    if (!my_loc.walls[2] && my_loc.can_move_to(south_loc) && !south_loc.visited) {
      State* south_state = new State(south_loc, next_state, (2 - next_state->cur_dir) % 4, 2);
      frontier.enqueue(south_state);
    }

    if (!my_loc.walls[3] && my_loc.can_move_to(west_loc) && !west_loc.visited) {
      State* west_state = new State(west_loc, next_state, (3 - next_state->cur_dir) % 4, 3);
      frontier.enqueue(west_state);
    }
  }

  return NULL;  // if no solution is found
}

// Function to execute the shortest path
void execute_shortest_path(State* sol) {
  while (sol->parent != sol) {
    act_stack.push(sol->action);
    mark_bktrk_api(sol->location.position);
    sol = sol->parent;
  }

  while (!act_stack.isEmpty()) {
    int act = act_stack.pop();
    mark_solution_api();
    if (act == 1) {
      turn_right();
    }
    else if (act == 3) {
      turn_left();
    }
    move_forward();
  }
}
