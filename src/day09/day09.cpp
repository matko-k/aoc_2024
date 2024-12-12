#include "day09.h"
#include <commons.h>
#include <unordered_map>
#include <unordered_set>

static const std::string DAY = "day09";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static std::string parseInput(const std::string &file_name) {
  std::ifstream file(file_name);
  std::string line;
  std::getline(file, line);
  file.close();

  return line;
}

std::vector<long> formatInput(const std::string &input) {
  std::vector<long> out;
  int id = 0;
  bool is_block = true;

  for (char c : input) {
    long num = c - '0';

    if (is_block) {
      for (int i = 1; i <= num; i++) {
        out.push_back(id);
      }
      id++;
    } else {
      for (int i = 1; i <= num; i++) {
        out.push_back(-1L);
      }
    }

    is_block = !is_block;
  }

  return out;
}

void day09::runDay09Part1() {
  const auto &input = parseInput(INPUT_PATH);
  auto memory = formatInput(input);

  auto start = memory.begin();
  auto end = memory.end() - 1;

  while (start < end) {
    while (start < end && *start != -1) {
      start++;
    }

    while (start < end && *end == -1) {
      end--;
    }

    if (start < end) {
      std::iter_swap(start, end);
    }
  }

  long res = 0;

  for (long i = 0; i < memory.size(); i++) {
    if (memory[i] == -1)
      break;
    res += memory[i] * i;
  }

  std::cout << "Day09 Part1: " << res << "\n";
}

void day09::runDay09Part2() {
  const auto &input = parseInput(INPUT_PATH);
  auto memory = formatInput(input);

  auto rit = memory.rbegin();
  auto it = memory.begin();
  while (true) {

    rit = std::find_if(rit, memory.rend(), [](long x) { return x != -1; });

    int rlength = 0;

    for (int i = 0; (rit + i) != memory.rend() && *rit == *(rit + i); i++) {
      rlength++;
    }

    auto end = (rit + rlength).base();

    while (it < end) {
      it = std::find(it, end, -1);

      if (it >= end)
        break;

      int gap_length = 0;

      for (int i = 0; (it + i) != end && *(it + i) == -1; i++) {
        gap_length++;
      }

      if (gap_length >= rlength) {
        break;
      }

      it += gap_length + 1;
    }
    if (rit.base() <= it)
      break;

    if (it <= (rit + rlength).base()) {
      for (int i = 0; i < rlength; i++)
        std::iter_swap(it + i, (rit + rlength).base() + i);
    }

    rit += rlength;
    it = memory.begin();
  }

  long res = 0;

  for (long i = 0; i < memory.size(); i++) {
    if (memory[i] == -1)
      continue;
    res += memory[i] * i;
  }

  std::cout << "Day09 Part2: " << res << "\n";
}