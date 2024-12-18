#include "day18.h"
#include "commons.h"
#include <queue>
#include <unordered_set>

static const std::string DAY = "day18";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name, std::vector<Pos> &out) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string part;
    Pos p;
    std::getline(ss, part, ',');
    p.x = std::stoi(part);
    std::getline(ss, part, ',');
    p.y = std::stoi(part);

    out.push_back(p);
  }

  file.close();
}

bool isValid(const std::vector<std::string> &map, const Pos &pos) {
  return pos.x >= 0 && pos.x < map[0].size() && pos.y >= 0 &&
         pos.y < map.size() && map[pos.y][pos.x] == '.';
}

int dijkstra(const std::vector<std::string> &map, const Pos &start,
             const Pos &goal, std::vector<Pos> &path) {
  std::unordered_map<Pos, int> g_cost;
  std::unordered_map<Pos, Pos> parent;

  g_cost[start] = 0;

  std::priority_queue<std::pair<int, Pos>, std::vector<std::pair<int, Pos>>,
                      std::greater<>>
      open_list;
  open_list.emplace(0, start);

  const std::vector<Pos> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  while (!open_list.empty()) {
    auto [cost, current] = open_list.top();
    open_list.pop();

    if (current == goal) {
      Pos p = current;
      while (p != start) {
        path.push_back(p);
        p = parent[p];
      }
      path.push_back(p);
      return cost;
    }

    for (const Pos &dir : dirs) {
      Pos next = {current.x + dir.x, current.y + dir.y};

      if (isValid(map, next)) {
        int next_cost = cost + 1;

        if (!g_cost.contains(next) || next_cost < g_cost[next]) {
          g_cost[next] = next_cost;
          parent[next] = current;
          open_list.emplace(next_cost, next);
        }
      }
    }
  }

  return -1;
}

void day18::runDay18Part1() {
  std::vector<Pos> bytes;
  parseInput(INPUT_PATH, bytes);

  //  int width = 7;
  //  int height = 7;
  //  int bytes_count = 12;
  int width = 71;
  int height = 71;
  int bytes_count = 1024;

  std::vector<std::string> map = std::vector(height, std::string(width, '.'));
  for (int i = 0; i < bytes_count; i++)
    map[bytes[i].y][bytes[i].x] = '#';

  std::vector<Pos> path;
  int path_len = dijkstra(map, {0, 0}, {width - 1, height - 1}, path);

  int res = path_len;

  std::cout << "Day18 Part1: " << res << "\n";
}

void day18::runDay18Part2() {
  std::vector<Pos> bytes;
  parseInput(INPUT_PATH, bytes);

  //  int width = 7;
  //  int height = 7;
  //  int bytes_count = 12;
  int width = 71;
  int height = 71;
  int bytes_count = 1024;

  std::vector<std::string> map = std::vector(height, std::string(width, '.'));
  for (int i = 0; i < bytes_count; i++)
    map[bytes[i].y][bytes[i].x] = '#';

  Pos byte;
  std::unordered_set<Pos> last_path;
  for (int i = bytes_count; i < bytes.size(); i++) {
    byte = bytes[i];
    map[byte.y][byte.x] = '#';

    if (!last_path.empty() && !last_path.contains(byte))
      continue;

    std::vector<Pos> path;
    int path_len = dijkstra(map, {0, 0}, {width - 1, height - 1}, path);
    if (path_len == -1)
      break;
    last_path = std::unordered_set<Pos>(path.begin(), path.end());
  }

  std::string res = std::to_string(byte.x) + ',' + std::to_string(byte.y);

  std::cout << "Day18 Part2: " << res << "\n";
}