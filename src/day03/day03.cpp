#include "day03.h"
#include <regex>

static const std::string DAY = "day03";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::string parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;
  std::string out;
  while (std::getline(file, line)) {
    out += line;
  }
  file.close();

  return out;
}

void day03::runDay03Part1() {
  const auto &input = parseInput(INPUT_PATH);

  long res = 0;
  std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

  const auto begin = std::sregex_iterator(input.begin(), input.end(), pattern);
  const auto end = std::sregex_iterator();

  for (auto it = begin; it != end; it++) {
    const std::smatch &match = *it;
    res += std::stol(match[1].str()) * std::stol(match[2].str());
  }
  std::cout << "Day03 Part1: " << res << "\n";
}

void day03::runDay03Part2() {
  const auto &input = parseInput(INPUT_PATH);

  long res = 0;
  std::regex pattern(R"((mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\)))");

  const auto begin = std::sregex_iterator(input.begin(), input.end(), pattern);
  const auto end = std::sregex_iterator();
  int enabled = 1;

  for (auto it = begin; it != end; it++) {
    const std::smatch &match = *it;

    if (match.str().substr(0, 2) == "do") {
      if (match.str().length() > 4) {
        enabled = 0;
        continue;
      }
      enabled = 1;
      continue;
    }
    res += enabled * std::stol(match[2].str()) * std::stol(match[3].str());
  }
  std::cout << "Day03 Part2: " << res << "\n";
}