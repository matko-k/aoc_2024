#include "day12.h"
#include <commons.h>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

static const std::string DAY = "day12";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string EXAMPLE_PATH_2 = "../inputs/" + DAY + "/example2.txt";
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

void findNeighbors(const std::vector<std::string> &map, const Pos &pos,
                   char plant, std::vector<Pos> &neighbors) {
  std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (const Pos dir : dirs) {
    if (pos.x + dir.x < 0 || pos.x + dir.x >= map[0].size() ||
        pos.y + dir.y < 0 || pos.y + dir.y >= map.size())
      continue;
    if (map[pos.y + dir.y][pos.x + dir.x] == plant)
      neighbors.push_back({pos.x + dir.x, pos.y + dir.y});
  }
}

std::vector<Pos> countSides(const std::vector<std::string> &map, const Pos &pos,
                            char plant) {
  std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  std::vector<Pos> sides;
  for (const Pos dir : dirs) {
    if (pos.x + dir.x < 0 || pos.x + dir.x >= map[0].size() ||
        pos.y + dir.y < 0 || pos.y + dir.y >= map.size() ||
        map[pos.y + dir.y][pos.x + dir.x] != plant)
      sides.push_back(dir);
  }
  return sides;
}

void day12::runDay12Part1() {
  const auto &input = parseInput(INPUT_PATH);

  std::vector<std::vector<int>> visited(input.size(),
                                        std::vector<int>(input[0].size(), 0));

  int res = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++) {
      if (visited[i][j] == 1)
        continue;

      std::array<int, 5> sides_count = {0, 0, 0, 0, 0};
      const char plant = input[i][j];
      std::vector<Pos> neighbors = {{j, i}};
      while (!neighbors.empty()) {
        Pos current = neighbors.back();
        neighbors.pop_back();
        if (visited[current.y][current.x] == 1)
          continue;

        const auto &sides = countSides(input, current, plant);
        sides_count[sides.size()] += 1;
        visited[current.y][current.x] = 1;

        findNeighbors(input, current, plant, neighbors);
      }

      res += (std::accumulate(sides_count.begin(), sides_count.end(), 0)) *
             (sides_count[1] * 1 + sides_count[2] * 2 + sides_count[3] * 3 +
              sides_count[4] * 4);
    }
  }

  std::cout << "Day12 Part1: " << res << "\n";
}

void day12::runDay12Part2() {
  const auto &input = parseInput(INPUT_PATH);

  std::vector<std::vector<int>> visited(input.size(),
                                        std::vector<int>(input[0].size(), 0));

  int res = 0;

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++) {
      if (visited[i][j] == 1)
        continue;

      const char plant = input[i][j];
      std::vector<Pos> neighbors = {{j, i}};
      std::vector<Pos> area;

      while (!neighbors.empty()) {
        Pos current = neighbors.back();
        neighbors.pop_back();
        if (visited[current.y][current.x] == 1)
          continue;

        const auto &sides = countSides(input, current, plant);
        visited[current.y][current.x] = 1;
        area.push_back(current);

        findNeighbors(input, current, plant, neighbors);
      }

      int corners = 0;
      int top_y = -1;
      int bottom_y = input.size();
      for (const Pos &p : area) {
        if (p.y < bottom_y)
          bottom_y = p.y;
        if (p.y > top_y)
          top_y = p.y;
      }

      int last_left_x = -1;
      int last_right_x = -1;
      std::unordered_map<char, std::vector<Pos>> inner_entries;

      for (int y = bottom_y; y <= top_y; y++) {
        int right_x = -1;
        int left_x = input[0].size();
        for (const Pos &p : area) {
          if (p.y != y)
            continue;
          if (p.x < left_x)
            left_x = p.x;
          if (p.x > right_x)
            right_x = p.x;
        }

        if (last_left_x != left_x)
          corners += 2;
        if (last_right_x != right_x)
          corners += 2;

        std::vector<int> plant_xs;
        for (int x = left_x; x <= right_x; x++) {
          Pos p = {x, y};
          if (std::find(area.begin(), area.end(), p) == area.end()) {
            plant_xs.push_back(x);
          }
        }

        for (int x = left_x; x <= right_x; x++) {
          Pos p = {x, y};
          Pos p_left = {x - 1, y};
          Pos p_right = {x + 1, y};
          if (std::find(area.begin(), area.end(), p) == area.end() &&
              (std::find(area.begin(), area.end(), p_left) != area.end() ||
               std::find(area.begin(), area.end(), p_right) != area.end())) {

            inner_entries[input[y][x]].push_back({x, y});
          }
        }

        last_left_x = left_x;
        last_right_x = right_x;
      }

      for (const auto &ie : inner_entries) {
        std::vector<Pos> perimeter = ie.second;
        if (perimeter.size() <= 4)
          corners += 4;
        else {
          int added_corners = 2;
          for (const Pos &pos : perimeter) {
            Pos p_up = {pos.x, pos.y - 1};
            Pos p_down = {pos.x, pos.y + 1};
            if (std::find(perimeter.begin(), perimeter.end(), p_up) !=
                    perimeter.end() &&
                std::find(perimeter.begin(), perimeter.end(), p_down) !=
                    perimeter.end())
              continue;
            added_corners++;
            if (input[p_down.y][p_down.x] == plant)
              added_corners--;
          }
          corners += added_corners;
        }
      }

      res += area.size() * corners;
    }
  }
  std::cout << "Day12 Part2: " << res << "\n";
}