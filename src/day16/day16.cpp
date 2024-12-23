#include "day16.h"
#include "commons.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>

static const std::string DAY = "day16";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::vector<std::string> &map) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    map.push_back(line);
  }

  file.close();
}

struct Pose {
  Pos pos;
  Pos orient;

  bool operator==(const Pose &other) const {
    return pos == other.pos && orient == other.orient;
  }

  bool operator<(const Pose &other) const {
    if (pos.x == other.pos.x) {
      if (pos.y == other.pos.y) {
        if (orient.x == other.orient.x) {
          return orient.y < other.orient.y;
        }
        return orient.x < other.orient.x;
      }
      return pos.y < other.pos.y;
    }
    return pos.x < other.pos.x;
  }
};

template <> struct std::hash<Pose> {
  size_t operator()(const Pose &p) const {
    return std::hash<long>()(p.pos.x) ^ std::hash<long>()(p.pos.y) ^
           std::hash<long>()(p.orient.x) ^ std::hash<long>()(p.orient.y);
  }
};

long findPath(const std::vector<std::string> &map, Pose start, Pose goal,
              std::unordered_set<Pos> &path, bool part_2) {

  std::priority_queue<std::tuple<int, Pose, std::vector<Pos>>,
                      std::vector<std::tuple<int, Pose, std::vector<Pos>>>,
                      std::greater<>>
      open_list;
  std::unordered_map<Pose, int> g_cost;

  open_list.push({0, start, {start.pos}});
  long best_path_score = 10e9;

  int best_paths = 0;

  while (!open_list.empty()) {
    auto [cost, current, current_poses] = open_list.top();
    open_list.pop();

    if (current.pos == goal.pos) {
      int path_cost = cost;

      if (path_cost <= best_path_score) {
        for (const Pos &p : current_poses)
          path.insert(p);
        if (!part_2)
          return path_cost;
        best_path_score = path_cost;
        best_paths++;
        continue;
      }
      return best_path_score;
    }

    if (g_cost.contains(current) && (!part_2 || g_cost[current] < cost))
      continue;
    g_cost[current] = cost;

    Pos next_pos = {current.pos.x + current.orient.x,
                    current.pos.y + current.orient.y};
    Pose next = {next_pos, current.orient};
    int rotation_cost = cost + 1000;
    int movement_cost = cost + 1;

    // move

    if (next_pos.x >= 0 && next_pos.x < map[0].size() && next_pos.y >= 0 &&
        next_pos.y < map.size() && map[next_pos.y][next_pos.x] == '.') {
      if (!g_cost.contains(next) || cost < g_cost[next]) {
        std::vector<Pos> new_poses = current_poses;
        new_poses.push_back(next.pos);
        open_list.push({movement_cost, next, new_poses});
        if (part_2)
          g_cost[next] = movement_cost;
      }
    }

    // rotate right

    Pose rotated_right = {current.pos, {current.orient.y, -current.orient.x}};
    if (!g_cost.contains(rotated_right) || cost < g_cost[rotated_right]) {
      open_list.push({rotation_cost, rotated_right, current_poses});
      if (part_2)
        g_cost[rotated_right] = rotation_cost;
    }

    // rotate left

    Pose rotated_left = {current.pos, {-current.orient.y, current.orient.x}};
    if (!g_cost.contains(rotated_left) || cost < g_cost[rotated_left]) {
      open_list.push({rotation_cost, rotated_left, current_poses});
      if (part_2)
        g_cost[rotated_left] = rotation_cost;
    }
  }
  return -1;
}

void day16::runDay16Part1() {
  std::vector<std::string> map;
  parseInput(INPUT_PATH, map);

  Pose start{1, static_cast<int>(map.size() - 2), 1, 0};
  Pose goal{static_cast<int>(map[0].size() - 2), 1, 1, 0};
  map[start.pos.y][start.pos.x] = '.';
  map[goal.pos.y][goal.pos.x] = '.';

  std::unordered_set<Pos> path;
  long res = findPath(map, start, goal, path, false);

  std::cout << "Day16 Part1: " << res << "\n";
}

void day16::runDay16Part2() {
  std::vector<std::string> map;
  parseInput(INPUT_PATH, map);

  Pose start{1, static_cast<int>(map.size() - 2), 1, 0};
  Pose goal{static_cast<int>(map[0].size() - 2), 1, 1, 0};
  map[start.pos.y][start.pos.x] = '.';
  map[goal.pos.y][goal.pos.x] = '.';

  std::unordered_set<Pos> path;
  findPath(map, start, goal, path, true);

  int res = path.size();

  std::cout << "Day16 Part2: " << res << "\n";
}