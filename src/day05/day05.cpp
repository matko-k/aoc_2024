#include "day05.h"
#include <map>

static const std::string DAY = "day05";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(std::unordered_map<int, std::vector<int>> &rules,
                       std::vector<std::vector<int>> &updates,
                       const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty())
      break;
    std::istringstream line_stream(line);
    std::string part;

    std::getline(line_stream, part, '|');
    int left = std::stoi(part);
    std::getline(line_stream, part);
    int right = std::stoi(part);
    rules[left].push_back(right);
  }

  while (std::getline(file, line)) {
    std::vector<int> numbers;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) {
      numbers.push_back(std::stoi(token));
    }
    updates.push_back(numbers);
  }

  file.close();
}

void day05::runDay05Part1() {
  std::unordered_map<int, std::vector<int>> rules;
  std::vector<std::vector<int>> updates;
  parseInput(rules, updates, INPUT_PATH);

  int res = 0;

  for (const auto &update : updates) {
    bool is_valid = true;
    for (int i = 0; i < update.size() - 1; i++) {
      for (int j = i + 1; j < update.size(); j++) {
        std::vector<int> smaller_nums = rules[update[j]];
        if (std::find(smaller_nums.begin(), smaller_nums.end(), update[i]) !=
            smaller_nums.end()) {
          is_valid = false;
          break;
        }
      }
      if (!is_valid)
        break;
    }

    if (is_valid)
      res += update[update.size() / 2];
  }

  std::cout << "Day05 Part1: " << res << "\n";
}

void day05::runDay05Part2() {
  std::unordered_map<int, std::vector<int>> rules;
  std::vector<std::vector<int>> updates;
  parseInput(rules, updates, INPUT_PATH);

  int res = 0;

  for (const auto &update : updates) {
    bool is_valid = true;
    for (int i = 0; i < update.size() - 1; i++) {
      for (int j = i + 1; j < update.size(); j++) {
        std::vector<int> smaller_nums = rules[update[j]];
        if (std::find(smaller_nums.begin(), smaller_nums.end(), update[i]) !=
            smaller_nums.end()) {
          is_valid = false;
          break;
        }
      }
      if (!is_valid)
        break;
    }

    if (!is_valid) {
      std::map<int, int> smaller_than_counts;

      for (int num : update) {
        std::vector<int> smaller_nums = rules[num];
        for (int smaller_num : smaller_nums) {
          if (std::find(update.begin(), update.end(), smaller_num) !=
              update.end()) {
            smaller_than_counts[num]++;
          }
        }
      }

      std::vector<int> updated_update = update;
      std::sort(updated_update.begin(), updated_update.end(),
                [&smaller_than_counts](int left, int right) {
                  return smaller_than_counts[left] > smaller_than_counts[right];
                });

      res += updated_update[updated_update.size() / 2];
    }
  }

  std::cout << "Day05 Part2: " << res << "\n";
}