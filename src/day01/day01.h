#ifndef AOC_2024_DAY01_H
#define AOC_2024_DAY01_H

#include <commons.h>
#include <set>
#include <vector>

class day01 {
public:
  static std::vector<std::multiset<int>>
  parseInput(const std::string &file_name);
  static void runDay1Part1();
  static void runDay1Part2();
};

#endif // AOC_2024_DAY01_H
