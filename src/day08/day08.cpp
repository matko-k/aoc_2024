#include "day08.h"
#include <commons.h>
#include <unordered_map>
#include <unordered_set>

static const std::string DAY = "day08";
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

std::unordered_set<Pos> findAntinodes(const std::vector<Pos> &antennas,
                                      int width, int height) {
  std::unordered_set<Pos> antinodes;
  for (int i = 0; i < antennas.size(); i++) {
    for (int j = i + 1; j < antennas.size(); j++) {
      Pos a1 = antennas[i];
      Pos a2 = antennas[j];
      Pos an1 = {a1.x - (a2.x - a1.x), a1.y - (a2.y - a1.y)};
      if (an1.x >= 0 && an1.x < width && an1.y >= 0 && an1.y < height)
        antinodes.insert(an1);
      Pos an2 = {a2.x + (a2.x - a1.x), a2.y + (a2.y - a1.y)};
      if (an2.x >= 0 && an2.x < width && an2.y >= 0 && an2.y < height)
        antinodes.insert(an2);
    }
  }
  return antinodes;
}

std::unordered_set<Pos> findAntinodesAdvanced(const std::vector<Pos> &antennas,
                                              int width, int height) {
  std::unordered_set<Pos> antinodes;
  for (int i = 0; i < antennas.size(); i++) {
    for (int j = i + 1; j < antennas.size(); j++) {
      Pos a1 = antennas[i];
      Pos a2 = antennas[j];
      int dx = a2.x - a1.x;
      int dy = a2.y - a1.y;

      int an_x = a1.x;
      int an_y = a1.y;

      while (an_x < width && an_x >= 0 && an_y < height && an_y >= 0) {
        antinodes.insert({an_x, an_y});
        an_x -= dx;
        an_y -= dy;
      }

      an_x = a2.x;
      an_y = a2.y;

      while (an_x < width && an_x >= 0 && an_y < height && an_y >= 0) {
        antinodes.insert({an_x, an_y});
        an_x += dx;
        an_y += dy;
      }
    }
  }
  return antinodes;
}

void day08::runDay08Part1() {
  auto input = parseInput(INPUT_PATH);

  std::unordered_map<char, std::unordered_set<Pos>> antennas;

  for (int i = 0; i < input.size(); i++)
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] != '.')
        antennas[input[i][j]].insert({j, i});
    }

  int width = input[0].size();
  int height = input.size();
  std::unordered_set<Pos> unique_antinodes;

  for (const auto &antenna : antennas) {
    const auto &antinodes = findAntinodes(
        std::vector<Pos>(antenna.second.begin(), antenna.second.end()), width,
        height);
    for (const Pos &an : antinodes) {
      unique_antinodes.insert(an);
    }
  }

  int res = unique_antinodes.size();

  std::cout << "Day08 Part1: " << res << "\n";
}

void day08::runDay08Part2() {

  auto input = parseInput(INPUT_PATH);

  std::unordered_map<char, std::unordered_set<Pos>> antennas;

  for (int i = 0; i < input.size(); i++)
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] != '.')
        antennas[input[i][j]].insert({j, i});
    }

  int width = input[0].size();
  int height = input.size();
  std::unordered_set<Pos> unique_antinodes;

  for (const auto &antenna : antennas) {
    const auto &antinodes = findAntinodesAdvanced(
        std::vector<Pos>(antenna.second.begin(), antenna.second.end()), width,
        height);
    for (const Pos &an : antinodes) {
      unique_antinodes.insert(an);
    }
  }

  int res = unique_antinodes.size();

  std::cout << "Day08 Part2: " << res << "\n";
}