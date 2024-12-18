#include "day07.h"
#include <commons.h>
#include <unordered_map>

static const std::string DAY = "day07";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::unordered_map<long, std::vector<long>>
parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::unordered_map<long, std::vector<long>> out;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    long first_num;
    std::vector<long> others;
    char colon;
    ss >> first_num >> colon;

    long num;
    while (ss >> num) {
      others.push_back(num);
    }

    out[first_num] = others;
  }
  file.close();

  return out;
}

bool findEquation(const std::vector<long> &numbers, const long res,
                  long current_res, int next_index) {
  if (current_res > res)
    return false;

  if (next_index == numbers.size())
    return res == current_res;

  return findEquation(numbers, res, numbers[next_index] + current_res,
                      next_index + 1) ||
         findEquation(numbers, res, numbers[next_index] * current_res,
                      next_index + 1);
}

bool findEquationWithConcatenation(const std::vector<long> &numbers,
                                   const long res, long current_res,
                                   int next_index) {
  if (current_res > res)
    return false;

  if (next_index == numbers.size())
    return res == current_res;

  long multiplier = 1;
  while (multiplier <= numbers[next_index]) {
    multiplier *= 10; }
  long concat_num = current_res * multiplier + numbers[next_index];

  return findEquationWithConcatenation(
             numbers, res, numbers[next_index] + current_res, next_index + 1) ||
         findEquationWithConcatenation(
             numbers, res, numbers[next_index] * current_res, next_index + 1) ||
         findEquationWithConcatenation(numbers, res, concat_num,
                                       next_index + 1);
}

void day07::runDay07Part1() {
  auto input = parseInput(INPUT_PATH);

  long res = 0;

  for (const auto &entry : input) {
    long result = entry.first;
    std::vector<long> numbers = entry.second;

    if (findEquation(numbers, result, numbers[0], 1))
      res += result;
  }

  std::cout << "Day07 Part1: " << res << "\n";
}

void day07::runDay07Part2() {
  auto input = parseInput(INPUT_PATH);

  long res = 0;

  for (const auto &entry : input) {
    long result = entry.first;
    std::vector<long> numbers = entry.second;

    if (findEquationWithConcatenation(numbers, result, numbers[0], 1)) {
      res += result;
    }
  }

  std::cout << "Day07 Part2: " << res << "\n";
}