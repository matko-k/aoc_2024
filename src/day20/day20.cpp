#include "day20.h"
#include "commons.h"
#include <map>
#include <numeric>
#include <unordered_set>

static const std::string DAY = "day20";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::vector<std::string> &out) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    out.push_back(line);
  }

  file.close();
}

static bool isValid(const std::vector<std::string> &map, const Pos &pos) {
  return pos.x >= 0 && pos.x < map[0].size() && pos.y >= 0 &&
         pos.y < map.size() && map[pos.y][pos.x] == '.';
}

void findCheats(const std::vector<std::string> &map, const Pos &start,
                const Pos &goal, int CHEAT_MIN, std::map<int, int> &cheats,
                bool part_2) {
  const std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  std::unordered_map<Pos, int> distances;
  Pos current = start;
  int dist = 0;
  distances[start] = dist;
  while (current != goal) {
    for (const Pos &dir : dirs) {
      Pos next = {current.x + dir.x, current.y + dir.y};
      if (isValid(map, next) && !distances.contains(next)) {
        dist += 1;
        distances[next] = dist;
        current = next;
        break;
      }
    }
  }

  for (const auto &entry : distances) {
    Pos pos = entry.first;
    if (!part_2) {
      for (const Pos &dir : dirs) {

        Pos off_1 = {pos.x + dir.x, pos.y + dir.y};
        Pos off_2 = {pos.x + 2 * dir.x, pos.y + 2 * dir.y};
        if (!isValid(map, off_1) && isValid(map, off_2) &&
            distances[off_2] > entry.second) {
          int d = distances[off_2] - entry.second - 2;
          if (d >= CHEAT_MIN)
            cheats[d]++;
        }
      }
    } else {
      for (const auto &other : distances) {
        if (pos == other.first || other.second < entry.second)
          continue;

        int pos_dist = abs(pos.x - other.first.x) + abs(pos.y - other.first.y);
        if (pos_dist < 2 || pos_dist > 20)
          continue;

        int d = other.second - entry.second - pos_dist;
        if (d >= CHEAT_MIN)
          cheats[d]++;
      }
    }
  }
}

void day20::runDay20Part1() {
  std::vector<std::string> map;
  parseInput(INPUT_PATH, map);
  const int CHEAT_MIN = 100;

  Pos start;
  Pos goal;

  for (int i = 0; i < map.size(); i++)
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == 'S') {
        start = {j, i};
        map[i][j] = '.';
      }
      if (map[i][j] == 'E') {
        goal = {j, i};
        map[i][j] = '.';
      }
    }

  std::map<int, int> cheats;
  findCheats(map, start, goal, CHEAT_MIN, cheats, false);

  int res = 0;

  for (const auto &e : cheats)
    res += e.second;

  std::cout << "Day20 Part1: " << res << "\n";
}

void day20::runDay20Part2() {
  std::vector<std::string> map;
  parseInput(INPUT_PATH, map);

  const int CHEAT_MIN = 100;

  Pos start;
  Pos goal;

  for (int i = 0; i < map.size(); i++)
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == 'S') {
        start = {j, i};
        map[i][j] = '.';
      }
      if (map[i][j] == 'E') {
        goal = {j, i};
        map[i][j] = '.';
      }
    }

  std::map<int, int> cheats;
  findCheats(map, start, goal, CHEAT_MIN, cheats, true);

  int res = 0;

  for (const auto &e : cheats)
    res += e.second;
  std::cout << "Day20 Part2: " << res << "\n";
}