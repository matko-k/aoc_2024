#include "day11.h"
#include <commons.h>
#include <unordered_map>
#include <unordered_set>
#include <valarray>

static const std::string DAY = "day11";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

void doBlinking(std::unordered_map<long, long> &stones, int total_blinks) {
  int blinks = 0;
  while (blinks < total_blinks) {
    const std::unordered_map<long, long> current_stones = stones;
    std::unordered_map<long, long> new_stones;

    for (const auto &stone : current_stones) {
      if (stone.first == 0) {
        new_stones[1] += stone.second;
        continue;
      }

      int digits = static_cast<int>(log10(stone.first) + 1);
      if (digits % 2 == 0) {
        long divider = std::pow(10, digits / 2);
        long left = stone.first / divider;
        long right = stone.first % divider;
        new_stones[left] += stone.second;
        new_stones[right] += stone.second;

        continue;
      }

      new_stones[stone.first * 2024] += stone.second;
    }

    stones = new_stones;
    blinks++;
  }
}

void day11::runDay11Part1() {
  std::vector<long> input = {8793800, 1629, 65, 5, 960, 0, 138983, 85629};
  //  input = {125, 17}; // example

  std::unordered_map<long, long> stones;

  for (long stone : input) {
    stones[stone]++;
  }

  const int TOTAL_BLINKS = 25;
  doBlinking(stones, TOTAL_BLINKS);

  long res = 0;

  for (const auto &stone : stones)
    res += stone.second;

  std::cout << "Day11 Part1: " << res << "\n";
}

void day11::runDay11Part2() {
  std::vector<long> input = {8793800, 1629, 65, 5, 960, 0, 138983, 85629};

  std::unordered_map<long, long> stones;

  for (long stone : input) {
    stones[stone]++;
  }

  const int TOTAL_BLINKS = 75;
  doBlinking(stones, TOTAL_BLINKS);

  long res = 0;

  for (const auto &stone : stones)
    res += stone.second;

  std::cout << "Day11 Part2: " << res << "\n";
}