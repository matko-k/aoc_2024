#include "day04.h"
#include <regex>

static const std::string DAY = "day04";
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

void day04::runDay04Part1() {
  const auto &input = parseInput(INPUT_PATH);
  int res = 0;

  for (int y = 0; y < input.size(); y++) {
    for (int x = 0; x < input[y].size(); x++) {
      if (input[y][x] == 'X') {

        for (int i : {-1, 0, 1}) {
          for (int j : {-1, 0, 1}) {
            if (i == 0 && j == 0)
              continue;

            bool is_valid_direction = true;
            for (int depth = 1; depth < 4; depth++) {
              if (x + depth * i < 0 || x + depth * i >= input[0].size() ||
                  y + depth * j < 0 || y + depth * j >= input.size()) {
                is_valid_direction = false;
                break;
              }
            }

            if (!is_valid_direction)
              continue;

            if (input[y + j][x + i] == 'M' &&
                input[y + 2 * j][x + 2 * i] == 'A' &&
                input[y + 3 * j][x + 3 * i] == 'S') {
              res++;
            }
          }
        }
      }
    }
  }

  std::cout << "Day04 Part1: " << res << "\n";
}

void day04::runDay04Part2() {
  const auto &input = parseInput(INPUT_PATH);
  int res = 0;
  for (int y = 1; y < input.size() - 1; y++) {
    for (int x = 1; x < input[y].size() - 1; x++) {
      if (input[y][x] == 'A') {
        char upper_left = input[y - 1][x - 1];
        char upper_right = input[y - 1][x + 1];
        char lower_left = input[y + 1][x - 1];
        char lower_right = input[y + 1][x + 1];

        bool are_chars_valid = (upper_left == 'M' || upper_left == 'S') &&
                               (upper_right == 'M' || upper_right == 'S') &&
                               (lower_left == 'M' || lower_left == 'S') &&
                               (lower_right == 'M' || lower_right == 'S');
        bool are_diagonals_different =
            (upper_left != lower_right) && (upper_right != lower_left);

        if (are_chars_valid && are_diagonals_different) {
          res++;
        }
      }
    }
  }

  std::cout << "Day04 Part2: " << res << "\n";
}