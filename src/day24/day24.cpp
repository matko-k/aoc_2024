#include "day24.h"
#include "commons.h"
#include <map>
#include <set>

static const std::string DAY = "day24";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

static void parseInput(const std::string &file_name,
                       std::unordered_map<std::string, int> &init_state,
                       std::vector<std::vector<std::string>> &operations) {
  std::ifstream file(file_name);
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty())
      break;
    std::string left = line.substr(0, 3);
    std::string right = line.substr(5, 1);
    init_state[left] = std::stoi(right);
  }

  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string left;
    std::string op;
    std::string right;
    std::string res;
    ss >> left >> op >> right >> res >> res;

    operations.push_back({left, op, right, res});
  }

  file.close();
}

int doOperation(const std::string &operation, int left, int right) {
  if (operation == "AND")
    return left & right;
  if (operation == "OR")
    return left | right;
  if (operation == "XOR")
    return left ^ right;

  return 0;
}

void day24::runDay24Part1() {
  std::unordered_map<std::string, int> wire_state;
  std::vector<std::vector<std::string>> operations;
  parseInput(INPUT_PATH, wire_state, operations);

  std::vector<std::vector<std::string>> ops_to_do = operations;
  int i = 0;
  while (!ops_to_do.empty()) {
    i %= ops_to_do.size();
    std::vector<std::string> next_op = ops_to_do[i];
    if (!wire_state.contains(next_op[0]) || !wire_state.contains(next_op[2]) ||
        wire_state.contains(next_op[3])) {
      i++;
      continue;
    }
    int res =
        doOperation(next_op[1], wire_state[next_op[0]], wire_state[next_op[2]]);
    wire_state[next_op[3]] = res;
    ops_to_do.erase(std::remove(ops_to_do.begin(), ops_to_do.end(), next_op),
                    ops_to_do.end());
    i++;
  }

  std::map<std::string, int> results_on_zs;
  for (const auto &state : wire_state) {
    if (state.first[0] != 'z')
      continue;
    results_on_zs.insert(state);
  }

  std::string res_bits;

  for (const auto &z : results_on_zs)
    res_bits += std::to_string(z.second);

  std::reverse(res_bits.begin(), res_bits.end());
  long long res = std::stoll(res_bits, nullptr, 2);
  ;

  std::cout << "Day24 Part1: " << res << "\n";
}

void day24::runDay24Part2() {
  std::unordered_map<std::string, int> wire_state;
  std::vector<std::vector<std::string>> operations;
  parseInput(INPUT_PATH, wire_state, operations);

  std::map<std::pair<std::set<std::string>, std::string>, std::string>
      sorted_ops;

  for (const auto &op : operations) {
    std::set<std::string> operands = {op[0], op[2]};
    auto key = std::make_pair(operands, op[1]);
    sorted_ops[key] = op[3];
  }

  std::set<std::string> swaps;
  std::string last_cout = "rnv";

  for (int i = 1; i < 45; i++) {
    std::string x_in =
        "x" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
    std::string y_in =
        "y" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
    std::string expected_z =
        "z" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i));

    std::map<std::string, std::pair<std::set<std::string>, std::string>>
        operations_for_inspection;
    std::string xor_1 = sorted_ops[{{x_in, y_in}, "XOR"}];
    operations_for_inspection[xor_1] = {{x_in, y_in}, "XOR"};

    std::string and_1 = sorted_ops[{{x_in, y_in}, "AND"}];
    operations_for_inspection[and_1] = {{x_in, y_in}, "AND"};

    std::string z_out = sorted_ops[{{xor_1, last_cout}, "XOR"}];
    operations_for_inspection[z_out] = {{xor_1, last_cout}, "XOR"};

    std::string and_2 = sorted_ops[{{xor_1, last_cout}, "AND"}];
    operations_for_inspection[and_2] = {{xor_1, last_cout}, "AND"};

    std::string cout = sorted_ops[{{and_1, and_2}, "OR"}];
    operations_for_inspection[cout] = {{and_1, and_2}, "OR"};

    if (!z_out.empty() && z_out != expected_z) {
      auto op1 = operations_for_inspection[z_out];
      auto op2 = operations_for_inspection[expected_z];
      sorted_ops[op1] = expected_z;
      sorted_ops[op2] = z_out;
      swaps.insert(expected_z);
      swaps.insert(z_out);
      i--;
      continue;
    }

    if (operations_for_inspection.contains("")) {
      std::vector<std::string> to_swap;
      for (const auto &op : operations_for_inspection) {
        if (op.first == "")
          continue;
        to_swap.push_back(op.first);
      }
      auto op1 = operations_for_inspection[to_swap[0]];
      auto op2 = operations_for_inspection[to_swap[1]];
      sorted_ops[op1] = to_swap[1];
      sorted_ops[op2] = to_swap[0];
      swaps.insert(to_swap[0]);
      swaps.insert(to_swap[1]);
      i--;
      continue;
    }

    last_cout = cout;
  }

  std::string res;
  for (const std::string &s : swaps)
    res += s + ',';
  res.pop_back();

  std::cout << "Day24 Part2: " << res << "\n";
}