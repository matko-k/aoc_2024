#include "day25.h"
#include "commons.h"
#include <map>
#include <set>

static const std::string DAY = "day25";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::set<std::vector<int>> &locks,
                       std::set<std::vector<int>> &keys) {
  std::ifstream file(file_name);
  std::string line;

  std::vector<std::string> temp;

  while (std::getline(file, line)) {
    if (line.empty()) {
      std::vector<int> comb;
      for (int i = 0; i < temp[0].size(); i++) {
        int count = 0;
        for (int j = 0; j < temp.size(); j++)
          if (temp[j][i] == '#')
            count++;
        comb.push_back(count - 1);
      }
      if (temp[0][0] == '.')
        keys.insert(comb);
      else
        locks.insert(comb);

      temp.clear();
      continue;
    }
    temp.push_back(line);
  }

  std::vector<int> comb;
  for (int i = 0; i < temp[0].size(); i++) {
    int count = 0;
    for (int j = 0; j < temp.size(); j++)
      if (temp[j][i] == '#')
        count++;
    comb.push_back(count - 1);
  }
  if (temp[0][0] == '.')
    keys.insert(comb);
  else
    locks.insert(comb);

  file.close();
}

void day25::runDay25Part1() {
  std::set<std::vector<int>> locks;
  std::set<std::vector<int>> keys;
  parseInput(INPUT_PATH, locks, keys);

  int res = 0;

  for (const auto &lock : locks)
    for (const auto &key : keys) {
      bool fits = true;
      for (int i = 0; i < key.size(); i++) {
        if (key[i] + lock[i] > 5) {
          fits = false;
          break;
        }
      }
      if (fits)
        res++;
    }
  std::cout << "Day25 Part1: " << res << "\n";
}

void day25::runDay25Part2() {
  std::set<std::vector<int>> locks;
  std::set<std::vector<int>> keys;
  parseInput(EXAMPLE_PATH, locks, keys);

  int res = 0;

  std::cout << "Day25 Part2: " << res << "\n";
}