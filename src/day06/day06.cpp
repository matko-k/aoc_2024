#include "day06.h"
#include <commons.h>
#include <unordered_set>

static const std::string DAY = "day06";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::vector<std::string> parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;
  std::vector<std::string> out;
  while (std::getline(file, line)) {
    out.push_back(line);
  }
  file.close();

  return out;
}

bool moveGuard(const std::vector<std::string> &map,
               std::unordered_set<Pos> &visited, Pos &current, Pos &dir) {
  visited.insert(current);
  Pos next = {current.x + dir.x, current.y + dir.y};

  if (next.x < 0 || next.x >= map[0].size() || next.y < 0 ||
      next.y >= map.size())
    return false;

  if (map[next.y][next.x] == '#') {
    dir = {-dir.y, dir.x};
    next = current;
  }

  return moveGuard(map, visited, next, dir);
}

bool moveGuardAndCheckForLoop(
    const std::vector<std::string> &map,
    std::unordered_map<Pos, std::unordered_set<Pos>> &visited, Pos &current,
    Pos &dir) {

  if (visited.find(current) != visited.end() &&
      visited[current].count(dir))
    return true;

  visited[current].insert(dir);

  Pos next = {current.x + dir.x, current.y + dir.y};

  if (next.x < 0 || next.x >= map[0].size() || next.y < 0 ||
      next.y >= map.size())
    return false;

  if (map[next.y][next.x] == '#') {
    dir = {-dir.y, dir.x};
    next = current;
  }

  return moveGuardAndCheckForLoop(map, visited, next, dir);
}

void day06::runDay06Part1() {
  auto map = parseInput(INPUT_PATH);

  Pos guard_pos = {0, 0};
  for (size_t y = 0; y < map.size(); y++) {
    const auto &row = map[y];
    for (size_t x = 0; x < row.size(); x++) {
      if (row[x] == '^') {
        map[y][x] = '.';
        guard_pos = {static_cast<long long>(x), static_cast<long long>(y)};
        break;
      }
    }
    if (guard_pos.x != 0 && guard_pos.y != 0)
      break;
  }

  Pos dir = {0, -1};
  Pos current = guard_pos;
  std::unordered_set<Pos> visited = {};

  moveGuard(map, visited, current, dir);

  size_t res = visited.size();

  std::cout << "Day06 Part1: " << res << "\n";
}

void day06::runDay06Part2() {
  auto map = parseInput(INPUT_PATH);

  Pos guard_pos = {0, 0};
  for (size_t y = 0; y < map.size(); y++) {
    const auto &row = map[y];
    for (size_t x = 0; x < row.size(); x++) {
      if (row[x] == '^') {
        map[y][x] = '.';
        guard_pos = {static_cast<long long>(x), static_cast<long long>(y)};
        break;
      }
    }
    if (guard_pos.x != 0 && guard_pos.y != 0)
      break;
  }

  Pos dir = {0, -1};
  Pos current = guard_pos;
  std::unordered_set<Pos> visited = {};

  moveGuard(map, visited, current, dir);

  visited.erase(guard_pos);

  int res = 0;

  for (const Pos &possible_obstacle : visited) {
    Pos dir = {0, -1};
    Pos current = guard_pos;

    auto map_copy = map;
    map_copy[possible_obstacle.y][possible_obstacle.x] = '#';
    std::unordered_map<Pos, std::unordered_set<Pos>> new_visited;
    if (moveGuardAndCheckForLoop(map_copy, new_visited, current, dir))
      res++;
  }

  std::cout << "Day06 Part2: " << res << "\n";
}