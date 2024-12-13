#include "day10.h"
#include <commons.h>
#include <unordered_map>
#include <unordered_set>

static const std::string DAY = "day10";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::vector<std::vector<int>> parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::vector<std::vector<int>> out;
  std::string line;

  while (std::getline(file, line)) {
    std::vector<int> row;

    for (char c : line) {
      row.push_back(c - '0');
    }
    out.push_back(row);
  }

  return out;
}

void findPaths(const std::vector<std::vector<int>> &map, const Pos &pos,
               std::vector<Pos> &trail_ends) {
  if (map[pos.y][pos.x] == 9) {
    trail_ends.push_back(pos);
    return;
  }

  std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (const Pos &dir : dirs) {
    if (pos.x + dir.x < 0 || pos.x + dir.x >= map[0].size() ||
        pos.y + dir.y < 0 || pos.y + dir.y >= map.size())
      continue;

    Pos next_pos = {pos.x + dir.x, pos.y + dir.y};
    if (map[next_pos.y][next_pos.x] - map[pos.y][pos.x] == 1)
      findPaths(map, next_pos, trail_ends);
  }
}

void day10::runDay10Part1() {
  const auto &input = parseInput(INPUT_PATH);
  int res = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == 0) {
        std::vector<Pos> trail_ends;
        findPaths(input, {j, i}, trail_ends);
        res += std::unordered_set<Pos>(trail_ends.begin(), trail_ends.end())
                   .size();
      }
    }
  }

  std::cout << "Day10 Part1: " << res << "\n";
}

void day10::runDay10Part2() {
  const auto &input = parseInput(INPUT_PATH);
  int res = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] == 0) {
        std::vector<Pos> trail_ends;
        findPaths(input, {j, i}, trail_ends);
        res += (int)trail_ends.size();
      }
    }
  }
  std::cout << "Day10 Part2: " << res << "\n";
}