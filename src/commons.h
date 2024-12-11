#ifndef AOC_2024_COMMONS_H
#define AOC_2024_COMMONS_H

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

void measureExecutionTime(const std::function<void()> &func);

struct Pos {
  long x;
  long y;

  bool operator==(const Pos &other) const {
    return x == other.x && y == other.y;
  }
};

namespace std {
template <> struct hash<Pos> {
  std::size_t operator()(const Pos &pos) const {
    return std::hash<long>()(pos.x) ^ (std::hash<long>()(pos.y) << 1);
  }
};
}

#endif // AOC_2024_COMMONS_H
