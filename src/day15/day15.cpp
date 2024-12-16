#include "day15.h"
#include "commons.h"
#include <unordered_map>

static const std::string DAY = "day15";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::vector<std::string> &map, std::string &moves) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty())
      break;
    map.push_back(line);
  }

  while (std::getline(file, line)) {
    moves += line;
  }

  file.close();
}

void moveRobot(std::vector<std::string> &map, Pos &robot, const Pos &dir) {
  std::vector<Pos> boxes_to_move;
  bool can_move = false;
  int x = robot.x;
  int y = robot.y;
  while (true) {
    x += dir.x;
    y += dir.y;
    if (map[y][x] == '#') {
      break;
    }
    if (map[y][x] == '.') {
      can_move = true;
      break;
    }
    if (map[y][x] == 'O')
      boxes_to_move.push_back({x, y});
  }
  if (can_move) {
    if (!boxes_to_move.empty()) {
      map[robot.y + dir.y * 2][robot.x + dir.x * 2] = 'O';
      map[robot.y + dir.y * (2 + boxes_to_move.size() - 1)]
         [robot.x + dir.x * (2 + boxes_to_move.size() - 1)] = 'O';
    }
    map[robot.y][robot.x] = '.';
    robot.x += dir.x;
    robot.y += dir.y;
    map[robot.y][robot.x] = '@';
  }
}

void moveRobot2(std::vector<std::string> &map, Pos &robot, const Pos &dir) {
  std::vector<Pos> boxes_to_move;

  bool is_vertical = dir.x == 0;

  bool can_move = false;
  int x = robot.x;
  int y = robot.y;
  std::vector<Pos> front_boxes;

  while (true) {
    x += dir.x;
    y += dir.y;

    //==check blocked==
    bool is_blocked = false;
    if (is_vertical && !front_boxes.empty()) {
      for (const Pos &box : front_boxes) {
        if (map[y][box.x] == '#' || map[y][box.x + 1] == '#') {
          is_blocked = true;
          break;
        }
      }
    } else {
      is_blocked = map[y][x] == '#';
    }
    if (is_blocked)
      break;

    //==check free==
    bool is_free = true;
    if (is_vertical && !front_boxes.empty()) {
      for (const Pos &box : front_boxes) {
        if (map[y][box.x] != '.' || map[y][box.x + 1] != '.') {
          is_free = false;
          break;
        }
      }
    } else {
      is_free = map[y][x] == '.';
    }
    if (is_free) {
      can_move = true;
      break;
    }

    //==handle boxes==
    std::vector<Pos> new_front_boxes;
    if (is_vertical && !front_boxes.empty()) {
      for (const Pos &box : front_boxes) {
        if (map[y][box.x] == '[') {
          boxes_to_move.push_back({box.x, y});
          new_front_boxes.push_back({box.x, y});
        }
        if (map[y][box.x + 1] == '[') {
          boxes_to_move.push_back({box.x + 1, y});
          new_front_boxes.push_back({box.x + 1, y});
        }
        if (map[y][box.x] == ']') {
          boxes_to_move.push_back({box.x - 1, y});
          new_front_boxes.push_back({box.x - 1, y});
        }

        if (map[y][box.x + 1] == ']') {
          boxes_to_move.push_back({box.x, y});
          new_front_boxes.push_back({box.x, y});
        }
      }

    } else {
      if (map[y][x] == '[') {
        boxes_to_move.push_back({x, y});
        new_front_boxes.push_back({x, y});
      }
      if (map[y][x] == ']') {
        boxes_to_move.push_back({x - 1, y});
        new_front_boxes.push_back({x - 1, y});
      }
    }
    front_boxes = new_front_boxes;
  }
  if (can_move) {
    for (const Pos &box : boxes_to_move) {
      map[box.y][box.x] = '.';
      map[box.y][box.x + 1] = '.';
    }

    for (const Pos &box : boxes_to_move) {
      map[box.y + dir.y][box.x + dir.x] = '[';
      map[box.y + dir.y][box.x + dir.x + 1] = ']';
    }

    map[robot.y][robot.x] = '.';
    robot.x += dir.x;
    robot.y += dir.y;
    map[robot.y][robot.x] = '@';
  }
}

void day15::runDay15Part1() {
  std::vector<std::string> map;
  std::string moves;
  parseInput(INPUT_PATH, map, moves);

  Pos robot_pos = {-1, -1};
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == '@') {
        robot_pos = {j, i};
        break;
      }
    }
    if (robot_pos.x > 0)
      break;
  }

  const std::unordered_map<char, Pos> dirs = {
      {'^', {0, -1}}, {'v', {0, 1}}, {'<', {-1, 0}}, {'>', {1, 0}}};

  for (char move : moves) {
    moveRobot(map, robot_pos, dirs.at(move));
  }

  int res = 0;

  for (int i = 0; i < map.size(); i++)
    for (int j = 0; j < map[i].size(); j++)
      if (map[i][j] == 'O')
        res += i * 100 + j;

  std::cout << "Day15 Part1: " << res << "\n";
}

void day15::runDay15Part2() {
  std::vector<std::string> map;
  std::string moves;
  parseInput(INPUT_PATH, map, moves);

  std::vector<std::string> wider_map(map.size(),
                                     std::string(2 * map[0].size(), '.'));

  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == '#') {
        wider_map[i][j * 2] = '#';
        wider_map[i][j * 2 + 1] = '#';
      } else if (map[i][j] == 'O') {
        wider_map[i][j * 2] = '[';
        wider_map[i][j * 2 + 1] = ']';
      } else if (map[i][j] == '@') {
        wider_map[i][j * 2] = '@';
      }
    }
  }

  Pos robot_pos = {-1, -1};
  for (int i = 0; i < wider_map.size(); i++) {
    for (int j = 0; j < wider_map[i].size(); j++) {
      if (wider_map[i][j] == '@') {
        robot_pos = {j, i};
        break;
      }
    }
    if (robot_pos.x > 0)
      break;
  }

  const std::unordered_map<char, Pos> dirs = {
      {'^', {0, -1}}, {'v', {0, 1}}, {'<', {-1, 0}}, {'>', {1, 0}}};

  for (char move : moves) {
    moveRobot2(wider_map, robot_pos, dirs.at(move));
    //    std::cout << "move: " << move << "\n";
    //    for (const auto &row : wider_map)
    //      std::cout << row << "\n";
    //    std::cout << "----------------------------\n";
  }

  int res = 0;

  for (int i = 0; i < wider_map.size(); i++)
    for (int j = 0; j < wider_map[i].size(); j++)
      if (wider_map[i][j] == '[')
        res += i * 100 + j;

  std::cout << "Day15 Part2: " << res << "\n";
}