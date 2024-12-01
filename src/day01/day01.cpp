
#include "day01.h"

void day01::runDay1Part1() {
  const auto &input = parseInput(INPUT_PATH);
  const auto &left = input[0];
  const auto &right = input[1];

  int res = 0;
  auto it_left = left.begin();
  auto it_right = right.begin();

  while (it_left != left.end()) {
    res += abs(*it_left - *it_right);
    it_left++;
    it_right++;
  }

  std::cout << "Day1 Part1: " << res << "\n";
}

void day01::runDay1Part2() {
  const auto &input = parseInput(INPUT_PATH);
  const auto &left = input[0];
  const auto &right = input[1];

  int res = 0;
  auto it_left = left.begin();

  while (it_left != left.end()) {
    res += *it_left * (int)right.count(*it_left);
    it_left++;
  }

  std::cout << "Day1 Part2: " << res << "\n";
}

std::vector<std::multiset<int>>
day01::parseInput(const std::string &file_name) {
  std::ifstream file(file_name);

  std::multiset<int> left;
  std::multiset<int> right;

  int num1, num2;
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    ss >> num1 >> num2;

    left.insert(num1);
    right.insert(num2);
  }

  file.close();

  return {left, right};
}
