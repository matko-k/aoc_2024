#include "day12.h"
#include <commons.h>
#include <numeric>
#include <set>
#include <unordered_map>

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

  std::vector<std::string> input_x9(input.size() * 3,
                                    std::string(input[0].size() * 3, '.'));

  for (int i = 0; i < input.size(); i++)
    for (int j = 0; j < input.size(); j++)
      for (int k = 0; k < 3; k++)
        for (int l = 0; l < 3; l++)
          input_x9[3 * i + k][3 * j + l] = input[i][j];

  std::vector<std::vector<int>> visited(
      input_x9.size(), std::vector<int>(input_x9[0].size(), 0));

  int res = 0;
  const std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  const std::vector<Pos> dirs_diagonals = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  for (int i = 0; i < input_x9.size(); i++) {
    for (int j = 0; j < input_x9[0].size(); j++) {
      if (visited[i][j] == 1)
        continue;

      const char plant = input_x9[i][j];
      std::vector<Pos> neighbors = {{j, i}};
      std::set<Pos> area;

      while (!neighbors.empty()) {
        Pos current = neighbors.back();
        neighbors.pop_back();
        if (visited[current.y][current.x] == 1)
          continue;

        visited[current.y][current.x] = 1;
        area.insert(current);

        findNeighbors(input_x9, current, plant, neighbors);
      }

      std::set<Pos> perimeter;

      for (const Pos &p : area) {
        for (const Pos &dir : dirs_diagonals) {
          Pos test = {p.x + dir.x, p.y + dir.y};
          if (area.contains(test))
            continue;
          perimeter.insert(test);
        }
      }

      int corners = 0;

      for (const Pos &p : perimeter) {
        int horizontal_neighbours = 0;
        int vertical_neighbors = 0;

        if (perimeter.contains({p.x - 1, p.y}))
          horizontal_neighbours++;
        if (perimeter.contains({p.x + 1, p.y}))
          horizontal_neighbours++;
        if (perimeter.contains({p.x, p.y - 1}))
          vertical_neighbors++;
        if (perimeter.contains({p.x, p.y + 1}))
          vertical_neighbors++;

        if (horizontal_neighbours == 1 && vertical_neighbors == 1)
          corners++;
      }

      res += area.size() / 9 * corners;
    }
  }
  std::cout << "Day12 Part2: " << res << "\n";
}