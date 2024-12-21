#include "day21.h"
#include "commons.h"
#include <map>
#include <numeric>
#include <regex>
#include <unordered_set>

static const std::string DAY = "day21";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::vector<std::string> &out) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    out.push_back(line);
  }

  file.close();
}

static const std::vector<std::string> NUM_KEYPAD = {"789", "456", "123", "X0A"};
static const std::vector<std::string> ARROW_KEYPAD = {"X^A", "<v>"};

void code2NumKeys(const std::string &code,
                  std::unordered_map<char, Pos> num_key_poses,
                  std::string &key_sequence) {
  key_sequence = "";
  char current = 'A';
  for (char c : code) {
    int d_x = num_key_poses[c].x - num_key_poses[current].x;
    int d_y = num_key_poses[c].y - num_key_poses[current].y;

    if (num_key_poses[current].y == 3 && num_key_poses[c].x == 0) {
      key_sequence += std::string(-d_y, '^');
      d_y = 0;
    }

    if (num_key_poses[current].x == 0 && num_key_poses[c].y == 3) {
      key_sequence += std::string(d_x, '>');
      d_x = 0;
    }

    if (d_x < 0)
      key_sequence += std::string(-d_x, '<');
    if (d_y > 0)
      key_sequence += std::string(d_y, 'v');
    if (d_y < 0)
      key_sequence += std::string(-d_y, '^');
    if (d_x > 0)
      key_sequence += std::string(d_x, '>');

    key_sequence += 'A';
    current = c;
  }
}

void arrow2ArrowKeys(const std::string &code,
                     std::unordered_map<char, Pos> arrow_key_poses,
                     std::string &key_sequence) {
  key_sequence = "";
  char current = 'A';
  for (char c : code) {
    int d_x = arrow_key_poses[c].x - arrow_key_poses[current].x;
    int d_y = arrow_key_poses[c].y - arrow_key_poses[current].y;

    if (arrow_key_poses[current].y == 0 && c == '<') {
      key_sequence += std::string(d_y, 'v');
      d_y = 0;
    }

    if (current == '<' && arrow_key_poses[c].y == 0) {
      key_sequence += std::string(d_x, '>');
      d_x = 0;
    }

    if (d_x < 0)
      key_sequence += std::string(-d_x, '<');
    if (d_y > 0)
      key_sequence += std::string(d_y, 'v');
    if (d_y < 0)
      key_sequence += std::string(-d_y, '^');
    if (d_x > 0)
      key_sequence += std::string(d_x, '>');
    key_sequence += 'A';
    current = c;
  }
}

std::vector<std::string> splitChunk(const std::string &str) {
  std::vector<std::string> result;
  std::regex re("([^A]+)|([A]+)");
  std::sregex_iterator it(str.begin(), str.end(), re);
  std::sregex_iterator end;

  std::string currentChunk;

  while (it != end) {
    std::string match = (*it).str();
    if (!match.empty()) {
      if (match.find_first_not_of('A') == std::string::npos) {
        if (!result.empty()) {
          result.back() += match;
        }
      } else {
        result.push_back(match);
      }
    }
    it++;
  }

  return result;
}

void arrow2ArrowKeysWithMemo(
    const std::unordered_map<char, Pos> &arrow_key_poses,
    std::unordered_map<std::string, std::vector<std::string>>
        &key_generation_memo,
    std::unordered_map<std::string, long> &key_sequence_count) {

  std::unordered_map<std::string, long> new_counts;

  for (const auto &entry : key_sequence_count) {
    if (key_generation_memo.contains(entry.first)) {
      for (const auto &child : key_generation_memo[entry.first]) {
        new_counts[child] += entry.second;
      }
    } else {
      std::string new_chunk;
      arrow2ArrowKeys(entry.first, arrow_key_poses, new_chunk);

      std::vector<std::string> new_chunks = splitChunk(new_chunk);
      key_generation_memo[entry.first] = new_chunks;

      for (const auto &c : new_chunks) {
        new_counts[c] += entry.second;
      }
    }
  }
  key_sequence_count = new_counts;
}

void day21::runDay21Part1() {
  std::vector<std::string> codes;
  parseInput(INPUT_PATH, codes);

  std::unordered_map<char, Pos> num_key_poses;
  for (int i = 0; i < NUM_KEYPAD.size(); i++) {
    for (int j = 0; j < NUM_KEYPAD[i].size(); j++) {
      num_key_poses[NUM_KEYPAD[i][j]] = {j, i};
    }
  }
  std::unordered_map<char, Pos> arrow_key_poses;
  for (int i = 0; i < ARROW_KEYPAD.size(); i++) {
    for (int j = 0; j < ARROW_KEYPAD[i].size(); j++) {
      arrow_key_poses[ARROW_KEYPAD[i][j]] = {j, i};
    }
  }

  std::vector<std::string> key_sequences;

  for (const std::string &code : codes) {
    std::string key_sequence;
    code2NumKeys(code, num_key_poses, key_sequence);
    for (int i = 0; i < 2; i++) {
      std::string new_code = key_sequence;
      arrow2ArrowKeys(new_code, arrow_key_poses, key_sequence);
    }
    key_sequences.push_back(key_sequence);
  }

  int res = 0;

  for (int i = 0; i < key_sequences.size(); i++) {
    res += key_sequences[i].size() *
           std::stoi(codes[i].substr(0, codes[i].size() - 1));
  }

  std::cout << "Day21 Part1: " << res << "\n";
}

void day21::runDay21Part2() {
  std::vector<std::string> codes;
  parseInput(INPUT_PATH, codes);

  std::unordered_map<char, Pos> num_key_poses;
  for (int i = 0; i < NUM_KEYPAD.size(); i++) {
    for (int j = 0; j < NUM_KEYPAD[i].size(); j++) {
      num_key_poses[NUM_KEYPAD[i][j]] = {j, i};
    }
  }
  std::unordered_map<char, Pos> arrow_key_poses;
  for (int i = 0; i < ARROW_KEYPAD.size(); i++) {
    for (int j = 0; j < ARROW_KEYPAD[i].size(); j++) {
      arrow_key_poses[ARROW_KEYPAD[i][j]] = {j, i};
    }
  }

  long res = 0;
  std::unordered_map<std::string, std::vector<std::string>> key_generation_memo;

  for (const std::string &code : codes) {
    std::vector<std::string> key_sequences;
    std::unordered_map<std::string, long> key_sequence_count;
    std::string key_sequence;
    code2NumKeys(code, num_key_poses, key_sequence);

    std::vector<std::string> chunks = splitChunk(key_sequence);
    for (const auto &chunk : chunks) {
      key_sequence_count[chunk] += 1L;
    }

    for (int i = 0; i < 25; i++) {
      arrow2ArrowKeysWithMemo(arrow_key_poses, key_generation_memo,
                              key_sequence_count);
    }

    long count = 0;
    for (const auto &entry : key_sequence_count) {
      count += entry.first.size() * entry.second;
    }

    res += count * std::stol(code.substr(0, code.size() - 1));
//    std::cout << count << " * " << std::stol(code.substr(0, code.size() - 1))
//              << "\n";
  }

  std::cout << "Day21 Part2: " << res << "\n";
}