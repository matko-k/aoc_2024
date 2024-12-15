#include "day13.h"
#include <cmath>
#include <commons.h>
#include <regex>
#include <unordered_map>

static const std::string DAY = "day13";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::vector<std::vector<long>> parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;
  std::vector<std::vector<long>> out;

  std::regex pattern(R"(X([+=])(\d+),?\s*Y\1(\d+))");
  std::vector<long> current;

  while (std::getline(file, line)) {
    if (line.empty()) {
      out.push_back(current);
      current.clear();
      continue;
    }

    std::smatch match;
    std::regex_search(line.cbegin(), line.cend(), match, pattern);
    current.push_back(std::stoi(match[2]));
    current.push_back(std::stoi(match[3]));
  }
  out.push_back(current);

  file.close();
  return out;
}

std::pair<double, double> solve_system(long a1, long b1, long c1, long a2,
                                       long b2, long c2) {
  double det = a1 * b2 - a2 * b1;

  if (det == 0) {
    return std::make_pair(-0.1, -0.1);
  }

  double det_x = c1 * b2 - c2 * b1;
  double det_y = a1 * c2 - a2 * c1;

  double x = det_x / det;
  double y = det_y / det;

  return {x, y};
}

void day13::runDay13Part1() {
  const auto &input = parseInput(INPUT_PATH);

  long res = 0;

  for (const auto &mach : input) {
    long a_x = mach[0];
    long a_y = mach[1];
    long b_x = mach[2];
    long b_y = mach[3];
    long goal_x = mach[4];
    long goal_y = mach[5];

    const auto &sol = solve_system(a_x, b_x, goal_x, a_y, b_y, goal_y);

    if (std::fmod(sol.first, 1.0) == 0.0 && std::fmod(sol.second, 1.0) == 0.0)
      res += 3 * sol.first + sol.second;
  }

  std::cout << "Day13 Part1: " << res << "\n";
}

void day13::runDay13Part2() {
  const auto &input = parseInput(INPUT_PATH);

  long res = 0;

  for (const auto &mach : input) {
    long a_x = mach[0];
    long a_y = mach[1];
    long b_x = mach[2];
    long b_y = mach[3];
    long goal_x = mach[4];
    long goal_y = mach[5];

    const auto &sol = solve_system(a_x, b_x, goal_x + 10000000000000, a_y, b_y,
                                   goal_y + 10000000000000);

    if (std::fmod(sol.first, 1.0) == 0.0 && std::fmod(sol.second, 1.0) == 0.0)
      res += 3 * sol.first + sol.second;
  }
  std::cout << "Day13 Part2: " << res << "\n";
}