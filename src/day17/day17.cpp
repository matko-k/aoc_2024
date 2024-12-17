#include "day17.h"
#include "commons.h"
#include <cmath>

static const std::string DAY = "day17";
static const std::string EXAMPLE_PATH = "../inputs/" + DAY + "/example.txt";
static const std::string INPUT_PATH = "../inputs/" + DAY + "/input.txt";

long getComboOperand(int operand, long &a_reg, long &b_reg, long &c_reg) {
  if (operand >= 0 && operand <= 3)
    return operand;
  if (operand == 4)
    return a_reg;
  if (operand == 5)
    return b_reg;
  if (operand == 6)
    return c_reg;
  return -1;
}

void doInstruction(int opcode, int operand, long &a_reg, long &b_reg,
                   long &c_reg, int &ip, std::string &out) {

  long combo_op = getComboOperand(operand, a_reg, b_reg, c_reg);
  if (opcode == 0) {
    a_reg = a_reg / std::pow(2, combo_op);
    return;
  }
  if (opcode == 1) {
    b_reg ^= operand;
    return;
  }
  if (opcode == 2) {
    b_reg = combo_op % 8;
    return;
  }
  if (opcode == 3) {
    if (a_reg == 0)
      return;
    ip = operand;
    return;
  }
  if (opcode == 4) {
    b_reg = c_reg ^ b_reg;
    return;
  }
  if (opcode == 5) {
    out += std::to_string(combo_op % 8L) + ',';
    return;
  }
  if (opcode == 6) {
    b_reg = a_reg / std::pow(2l, combo_op);
    return;
  }
  if (opcode == 7) {
    c_reg = a_reg / std::pow(2l, combo_op);
    return;
  }
}

void day17::runDay17Part1() {
  //  long    a_reg = 729;
  //  std::vector<int> program = {0, 1, 5, 4, 3, 0};

  long a_reg = 25358015;
  std::vector<int> program = {2, 4, 1, 1, 7, 5, 0, 3, 4, 7, 1, 6, 5, 5, 3, 0};
  long b_reg = 0;
  long c_reg = 0;

  std::string out;

  int ip = 0;

  while (ip < program.size()) {
    int new_ip = -1;

    doInstruction(program[ip], program[ip + 1], a_reg, b_reg, c_reg, new_ip,
                  out);

    ip = new_ip >= 0 ? new_ip : ip + 2;
  }

  out.pop_back();
  std::string res = out;

  std::cout << "Day17 Part1: " << res << "\n";
}

void day17::runDay17Part2() {
  //  long   a_reg = 2024;
  //  std::vector<int> program = {0, 3, 5, 4, 3, 0};

  long a_reg = 25358015;
  std::vector<int> program = {2, 4, 1, 1, 7, 5, 0, 3, 4, 7, 1, 6, 5, 5, 3, 0};
  long b_reg = 0;
  long c_reg = 0;

  std::string expected_out;
  for (int i : program) {
    expected_out += std::to_string(i) + ',';
  }
  expected_out.pop_back();

  long a_init = 0;
  long last_match = 0;

  int i = 0;
  std::vector<long> cycle = {1};
  long next_a_init = -1;
  std::vector<long> next_cycle;
  int check_length = 7;
  int check_cycle_size = 32;

  while (true) {

    a_reg = a_init;
    b_reg = 0;
    c_reg = 0;
    int ip = 0;

    std::string out;

    while (ip < program.size()) {
      int new_ip = -1;

      doInstruction(program[ip], program[ip + 1], a_reg, b_reg, c_reg, new_ip,
                    out);

      if (out.length() > 0)
        out.pop_back();

      if (out == expected_out)
        break;
      if (out.length() > expected_out.length() ||
          expected_out.compare(0, out.size(), out) != 0)
        break;

      if (out.length() > 0)
        out += ',';

      ip = new_ip >= 0 ? new_ip : ip + 2;
    }

    if (expected_out.substr(0, check_length)
            .compare(out.substr(0, check_length)) == 0) {
      if (next_a_init < 0)
        next_a_init = a_init;
      next_cycle.push_back(a_init - last_match);
      last_match = a_init;
    }

    if (out == expected_out)
      break;

    bool is_new_cycle = false;
    int next_cycle_len = 1;
    if (next_cycle.size() > check_cycle_size) {
      for (; next_cycle_len <= next_cycle.size() / 2; next_cycle_len++) {
        is_new_cycle = true;
        for (int j = 1; j < next_cycle.size() - next_cycle_len; j++) {
          if (next_cycle[j] != next_cycle[j + next_cycle_len]) {
            is_new_cycle = false;
            break;
          }
        }
        if (is_new_cycle) {
          break;
        }
      }
    }

    if (is_new_cycle) {
      a_init = next_a_init;
      next_a_init = -1;
      cycle.clear();
      std::copy(next_cycle.begin() + 1, next_cycle.begin() + next_cycle_len + 1,
                std::back_inserter(cycle));
      next_cycle.clear();
      check_length += 2;
      i = 0;
      check_cycle_size = 1024;
      continue;
    }

    a_init += cycle[i];
    i += 1;
    i %= (long)cycle.size();
  }

  long res = a_init;
  std::cout << "Day17 Part2: " << res << "\n";
}
