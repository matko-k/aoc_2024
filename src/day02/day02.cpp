#include "day02.h"

static const std::string DAY = "day02";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

void day02::runDay2Part1() {
  const auto &reports = parseInput(INPUT_PATH);

  int res = 0;

  for (const auto &report : reports) {
    res += isReportSafe(report);
  }

  std::cout << "Day2 Part1: " << res << "\n";
}

void day02::runDay2Part2() {
  const auto &reports = parseInput(INPUT_PATH);

  int res = 0;

  for (const auto &report : reports) {
    if (isReportSafe(report)) {
      res++;
      continue;
    }

    for (int i = 0; i < report.size(); ++i) {
      std::vector<int> copy = report;
      copy.erase(copy.begin() + i);
      if (isReportSafe(copy)) {
        res++;
        break;
      }
    }
  }

  std::cout << "Day2 Part2: " << res << "\n";
}

std::vector<std::vector<int>> day02::parseInput(const std::string &file_name) {
  std::ifstream file(file_name);

  std::vector<std::vector<int>> reports;

  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::vector<int> report = {};
    int num;
    while (ss >> num) {
      report.push_back(num);
    }
    reports.push_back(report);
  }

  file.close();

  return reports;
}

bool day02::isReportSafe(const std::vector<int> &report) {
  int last = report[0];
  int current = report[1];
  bool increasing = report[1] > report[0];
  int diff = abs(current - last);
  if (diff < 1 || diff > 3) {
    return false;
  }
  for (size_t i = 1; i < report.size(); i++) {
    last = report[i - 1];
    current = report[i];
    if (increasing != current > last) {
      return false;
    }
    int diff = abs(current - last);
    if (diff < 1 || diff > 3) {
      return false;
    }
  }
  return true;
}
