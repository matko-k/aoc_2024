#ifndef AOC_2024_DAY02_H
#define AOC_2024_DAY02_H

#include <commons.h>
#include <vector>

class day02 {
public:
  static std::vector<std::vector<int>> parseInput(const std::string &file_name);
  static void runDay2Part1();
  static void runDay2Part2();

private:
  static bool isReportSafe(const std::vector<int> &report);
};

#endif // AOC_2024_DAY02_H
