#ifndef AOC_2024_DAY01_H
#define AOC_2024_DAY01_H

#include <commons.h>
#include <set>
#include <vector>

static const std::string DAY = "day01";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

class day01 {
public:
  static std::vector<std::multiset<int>>
  parseInput(const std::string &file_name);
  static void runDay1Part1();
  static void runDay1Part2();
};

#endif // AOC_2024_DAY01_H
