#include "day22.h"
#include "commons.h"
#include <map>
#include <numeric>
#include <regex>
#include <unordered_set>

static const std::string DAY = "day22";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string EXAMPLE_PATH_2 = "../inputs/" + DAY + "/example2.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name, std::vector<long> &out) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    out.push_back(std::stol(line));
  }

  file.close();
}

long secretNum(long num) {
  const long init = num;
  long res = ((init * 64l) ^ init) % 16777216l;
  res = ((res / 32) ^ res) % 16777216l;
  res = ((res * 2048l) ^ res) % 16777216l;
  return res;
}

void day22::runDay22Part1() {
  std::vector<long> init_nums;
  parseInput(INPUT_PATH, init_nums);

  long res = 0;

  for (long num : init_nums) {
    long secret_num = num;
    for (int i = 0; i < 2000; i++) {
      secret_num = secretNum(secret_num);
    }
    res += secret_num;
  }

  std::cout << "Day22 Part1: " << res << "\n";
}

void day22::runDay22Part2() {
  std::vector<long> init_nums;
  parseInput(INPUT_PATH, init_nums);

  std::unordered_map<long, std::vector<long>> secret_nums;
  for (long num : init_nums) {
    long secret_num = num;
    secret_nums[num].push_back(secret_num);
    for (int i = 0; i < 2000; i++) {
      secret_num = secretNum(secret_num);
      secret_nums[num].push_back(secret_num);
    }
  }

  std::map<std::vector<int>, int> total_bananas;
  for (const auto &e : secret_nums) {
    std::map<std::vector<int>, int> bananas;
    for (int i = 1; i < e.second.size() - 4; i++) {
      std::vector<int> seq = {
          (int)e.second[i] % 10 - (int)e.second[i - 1] % 10,
          (int)e.second[i + 1] % 10 - (int)e.second[i + 1 - 1] % 10,
          (int)e.second[i + 2] % 10 - (int)e.second[i + 2 - 1] % 10,
          (int)e.second[i + 3] % 10 - (int)e.second[i + 3 - 1] % 10};

      if (bananas.contains(seq))
        continue;
      bananas[seq] = e.second[i + 3] % 10;
    }
    for (const auto &ban : bananas)
      total_bananas[ban.first] += ban.second;
  }

  int res = 0;
  for (const auto &ban : total_bananas) {
    res = std::max(res, ban.second);
  }

  std::cout << "Day22 Part2: " << res << "\n";
}