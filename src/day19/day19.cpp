#include "day19.h"
#include "commons.h"
#include <unordered_set>

static const std::string DAY = "day19";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::unordered_set<std::string> &patterns,
                       std::vector<std::string> &designs) {
  std::ifstream file(file_name);
  std::string line;
  std::getline(file, line);

  std::istringstream ss(line);
  std::string part;
  std::getline(ss, part, ',');
  patterns.insert(part);
  while (std::getline(ss, part, ','))
    patterns.insert(part.substr(1, part.length()));

  std::getline(file, line);

  while (std::getline(file, line)) {
    designs.push_back(line);
  }

  file.close();
}

bool isDesignPossible(const std::unordered_set<std::string> &initial_patterns,
                      std::unordered_set<std::string> &possible_cache,
                      std::unordered_set<std::string> &impossible_cache,
                      const std::string &design, int max_len) {
  if (design.empty())
    return true;

  if (possible_cache.contains(design))
    return true;

  if (impossible_cache.contains(design))
    return false;

  bool is_possible = false;
  for (int i = 1; i <= max_len && i <= design.size(); i++) {
    std::string chunk = design.substr(0, i);
    if (initial_patterns.contains(chunk)) {
      is_possible =
          isDesignPossible(initial_patterns, possible_cache, impossible_cache,
                           design.substr(i, design.length()), max_len);
    }
    if (is_possible) {
      possible_cache.insert(design);
      return true;
    }
  }
  impossible_cache.insert(design);
  return false;
}

long countPossibleDesigns(
    const std::unordered_set<std::string> &initial_patterns,
    std::unordered_map<std::string, long> &cache, const std::string &design,
    int max_len) {

  if (design.empty()) {
    return 1;
  }

  if (cache.contains(design))
    return cache[design];

  long count = 0;
  for (int i = 1; i <= max_len && i <= design.size(); i++) {
    std::string chunk = design.substr(0, i);
    if (initial_patterns.contains(chunk)) {
      count += countPossibleDesigns(initial_patterns, cache,
                                    design.substr(i, design.length()), max_len);
    }
  }
  cache[design] = count;
  return count;
}

void day19::runDay19Part1() {
  std::unordered_set<std::string> patterns;
  std::vector<std::string> designs;
  parseInput(INPUT_PATH, patterns, designs);

  int max_len = -1;

  for (const auto &p : patterns)
    max_len = std::max(max_len, (int)p.length());

  int res = 0;

  std::unordered_set<std::string> possible_cache = patterns;
  std::unordered_set<std::string> impossible_cache;
  for (const std::string &design : designs) {
    res += isDesignPossible(patterns, possible_cache, impossible_cache, design,
                            max_len)
               ? 1
               : 0;
  }

  std::cout << "Day19 Part1: " << res << "\n";
}

void day19::runDay19Part2() {
  std::unordered_set<std::string> patterns;
  std::vector<std::string> designs;
  parseInput(INPUT_PATH, patterns, designs);

  int max_len = -1;

  for (const auto &p : patterns)
    max_len = std::max(max_len, (int)p.length());

  long res = 0;

  std::unordered_map<std::string, long> cache;
  for (const std::string &design : designs) {
    res += countPossibleDesigns(patterns, cache, design, max_len);
  }
  std::cout << "Day19 Part2: " << res << "\n";
}