#include "day23.h"
#include "commons.h"
#include <set>

static const std::string DAY = "day23";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void
parseInput(const std::string &file_name,
           std::unordered_map<std::string, std::set<std::string>> &out) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    std::string left = line.substr(0, 2);
    std::string right = line.substr(3, 2);
    out[left].insert(right);
    out[right].insert(left);
  }

  file.close();
}

void day23::runDay23Part1() {
  std::unordered_map<std::string, std::set<std::string>> lan;
  parseInput(INPUT_PATH, lan);

  std::set<std::set<std::string>> counted;

  int res = 0;

  for (const auto &entry : lan) {

    for (auto it1 = entry.second.begin(); it1 != std::prev(entry.second.end());
         it1++)
      for (auto it2 = std::next(it1); it2 != entry.second.end(); it2++) {
        const std::string &left = *it1;
        const std::string &right = *it2;

        std::set<std::string> network = {entry.first, left, right};
        if (counted.contains(network))
          continue;
        counted.insert(network);

        if (lan[left].contains(right) && lan[left].contains(entry.first) &&
            lan[right].contains(left) && lan[right].contains(entry.first)) {
          if (entry.first[0] == 't' || left[0] == 't' || right[0] == 't')
            res++;
        }
      }
  }

  std::cout << "Day23 Part1: " << res << "\n";
}

void day23::runDay23Part2() {
  std::unordered_map<std::string, std::set<std::string>> lan;
  parseInput(INPUT_PATH, lan);

  std::vector<std::set<std::string>> subnets;
  std::set<std::set<std::string>> counted;

  for (const auto &entry : lan) {

    for (auto it1 = entry.second.begin(); it1 != entry.second.end(); it1++) {
      std::set<std::string> network = entry.second;
      network.insert(entry.first);
      network.erase(*it1);

      if (counted.contains(network))
        continue;
      counted.insert(network);

      bool all_connected = true;
      for (const std::string &node : network) {
        auto subnet = network;
        subnet.erase(node);

        if (!std::includes(lan[node].begin(), lan[node].end(), subnet.begin(),
                           subnet.end())) {
          all_connected = false;
          break;
        }
      }

      if (all_connected)
        subnets.push_back(network);
    }
  }

  std::string res;

  if (subnets.size() == 1)
    for (const std::string &s : subnets[0])
      res += s + ',';

  if (!res.empty())
    res.pop_back();

  std::cout << "Day23 Part2: " << res << "\n";
}