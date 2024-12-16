#include "day14.h"
#include <cmath>
#include <commons.h>
#include <regex>
#include <unordered_map>

static const std::string DAY = "day14";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::vector<std::vector<Pos>> parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;
  std::vector<std::vector<Pos>> out;

  while (std::getline(file, line)) {
    std::vector<Pos> robot;
    int px, py, vx, vy;
    char ignore;

    std::istringstream iss(line);

    if (iss >> ignore >> ignore >> px >> ignore >> py >> ignore >> ignore >>
        vx >> ignore >> vy) {
      robot.push_back({px, py});
      robot.push_back({vx, vy});
      out.push_back(robot);
    }
  }

  file.close();
  return out;
}

void moveRobot(const Pos &current_pos, const Pos &vel, int seconds, int width,
               int height, Pos &new_pos) {
  new_pos.x = (current_pos.x + vel.x * seconds) % width;
  if (new_pos.x < 0)
    new_pos.x += width;

  new_pos.y = (current_pos.y + vel.y * seconds) % height;
  if (new_pos.y < 0)
    new_pos.y += height;
}

int findQuadrant(const Pos &pos, int width, int height) {
  if (pos.x < width / 2 && pos.y < height / 2)
    return 0;
  if (pos.x < width / 2 && pos.y > height / 2)
    return 1;
  if (pos.x > width / 2 && pos.y > height / 2)
    return 2;
  if (pos.x > width / 2 && pos.y < height / 2)
    return 3;
  return 4;
}

void day14::runDay14Part1() {
  const auto &input = parseInput(INPUT_PATH);

  std::vector<int> quadrant_counts = {0, 0, 0, 0, 0};
  const int seconds = 100;
  const int width = 101;
  const int height = 103;
  for (const auto &robot : input) {
    Pos new_pos;
    moveRobot(robot[0], robot[1], seconds, width, height, new_pos);

    quadrant_counts[findQuadrant(new_pos, width, height)]++;
  }

  int res = quadrant_counts[0] * quadrant_counts[1] * quadrant_counts[2] *
            quadrant_counts[3];

  std::cout << "Day14 Part1: " << res << "\n";
}

void day14::runDay14Part2() {
  const auto &input = parseInput(INPUT_PATH);

  const int width = 101;
  const int height = 103;

  int res = 0;
  for (int i = 1; i <= 100000; i += 1) {
    std::vector<std::string> grid(height, std::string(width, '.'));

    const int seconds = i;
    for (const auto &robot : input) {
      Pos new_pos;
      moveRobot(robot[0], robot[1], seconds, width, height, new_pos);
      grid[new_pos.y][new_pos.x] = '#';
    }

    bool found = false;
    for (const auto &row : grid)
      if (row.find("########") != row.npos) {
        found = true;
        break;
      }

    if (found) {
      res = i;
      for (const auto &row : grid)
        std::cout << row << '\n';
      break;
    }
  }

  std::cout << "Day14 Part2: " << res << "\n";
}