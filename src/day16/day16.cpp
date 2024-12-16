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

int heuristic(const Pos &a, const Pos &b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int AStar(const std::vector<std::string> &map, Pose start, Pose goal,
          std::unordered_set<Pos> &path, bool part_2) {

  std::priority_queue<std::tuple<int, Pose, std::vector<Pose>>,
                      std::vector<std::tuple<int, Pose, std::vector<Pose>>>,
                      std::greater<std::tuple<int, Pose, std::vector<Pose>>>>
      open_list;
  std::unordered_map<Pose, int> g_cost;

  open_list.push({0, start, {start}});
  g_cost[start] = 0;
  long best_path_score = 10e9;

  int best_paths = 0;

  while (!open_list.empty()) {
    auto [cost, current, current_poses] = open_list.top();
    open_list.pop();

    if (current.pos == goal.pos) {
      int path_cost = g_cost[current];

      if (path_cost <= best_path_score) {
        for (const Pose &p : current_poses)
          path.insert(p.pos);
        if (!part_2)
          return path_cost;
        best_path_score = path_cost;
        best_paths++;
      }

      if (path_cost > best_path_score)
        return best_path_score;

      continue;
    }

    int rotation_cost = g_cost[current] + 1000;
    int movement_cost = g_cost[current] + 1;

    // rotate right
    {
      Pose rotated_right = current;
      rotated_right.orient = {current.orient.y, -current.orient.x};
      if (!g_cost.contains(rotated_right) ||
          rotation_cost < g_cost[rotated_right]) {
        g_cost[rotated_right] = rotation_cost;
        std::vector<Pose> new_poses = current_poses;
        new_poses.push_back(rotated_right);
        open_list.push({rotation_cost + heuristic(rotated_right.pos, goal.pos),
                        rotated_right, new_poses});
      } else if (part_2 && rotation_cost == g_cost[rotated_right]) {
        std::vector<Pose> new_poses = current_poses;
        new_poses.push_back(rotated_right);
        open_list.push({rotation_cost + heuristic(rotated_right.pos, goal.pos),
                        rotated_right, new_poses});
      }
    }

    // rotate left
    {
      Pose rotated_left = current;
      rotated_left.orient = {-current.orient.y, current.orient.x};
      if (!g_cost.contains(rotated_left) ||
          rotation_cost <= g_cost[rotated_left]) {
        g_cost[rotated_left] = rotation_cost;
        std::vector<Pose> new_poses = current_poses;
        new_poses.push_back(rotated_left);
        open_list.push({rotation_cost + heuristic(rotated_left.pos, goal.pos),
                        rotated_left, new_poses});
      } else if (part_2 && rotation_cost == g_cost[rotated_left]) {
        std::vector<Pose> new_poses = current_poses;
        new_poses.push_back(rotated_left);
        open_list.push({rotation_cost + heuristic(rotated_left.pos, goal.pos),
                        rotated_left, new_poses});
      }
    }

    // move
    {
      Pose moved = current;
      Pos moved_pos = {current.pos.x + current.orient.x,
                       current.pos.y + current.orient.y};

      if (moved_pos.x >= 0 && moved_pos.x < map[0].size() && moved_pos.y >= 0 &&
          moved_pos.y < map.size() && map[moved_pos.y][moved_pos.x] == '.') {
        moved.pos = moved_pos;
        if (!g_cost.contains(moved) || movement_cost < g_cost[moved]) {
          g_cost[moved] = movement_cost;
          std::vector<Pose> new_poses = current_poses;
          new_poses.push_back(moved);
          open_list.push({movement_cost + heuristic(moved.pos, goal.pos), moved,
                          new_poses});
        } else if (part_2 && movement_cost == g_cost[moved]) {
          std::vector<Pose> new_poses = current_poses;
          new_poses.push_back(moved);
          open_list.push({movement_cost + heuristic(moved.pos, goal.pos), moved,
                          new_poses});
        }
      }
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
  int res = AStar(map, start, goal, path, false);

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
  AStar(map, start, goal, path, true);

  int res = path.size();

  std::cout << "Day16 Part2: " << res << "\n";
}