// day_16.cpp : Part 1 and 2 solved

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

using reg_t = std::array<int, 4>;
using op_t = std::function<int(int, int)>;

struct inst_t {
  int op;
  int a;
  int b;
  int c;
};

struct dirty_op {
  unsigned int op_number = {};
  std::unordered_map<std::string, int> op_occurences = {};
};

struct op_exec {
  bool a_is_immediate;
  bool b_is_immediate;
  op_t op = {};
};

int add(const int a, const int b) { return a + b; }
int mult(const int a, const int b) { return a * b; }
int band(const int a, const int b) { return a & b; }
int bor(const int a, const int b) { return a | b; }
int greater(const int a, const int b) { return a > b; }
int equal(const int a, const int b) { return a == b; }
int set(const int a, const int b) { return a; }

auto matching_operations(const inst_t instr, const reg_t& reg_before, const reg_t& reg_after) 
{
  int count_op_match = 0;
  std::array<op_t, 7> ops = {add, mult, band, bor, greater, equal, set};

  auto A = reg_before[instr.a];  // Reg/reg instructions
  auto B = reg_before[instr.b];
  count_op_match += std::count_if(ops.begin(), ops.end(), [&](auto& op) {
    return reg_after[instr.c] == op(A, B);
  });

  A = reg_before[instr.a];  // Reg/immediate instructions
  B = instr.b;
  count_op_match += std::count_if(ops.begin(), ops.end() - 1, [&](auto& op) {
    return reg_after[instr.c] == op(A, B);
  });

  A = instr.a;  // Immediate/reg instructions
  B = reg_before[instr.b];
  count_op_match += std::count_if(ops.begin() + 4, ops.end(), [&](auto& op) {
    return reg_after[instr.c] == op(A, B);
  });
  return count_op_match;
}

auto part1() 
{
  auto res = 0;
  std::ifstream fs("input.txt");

  while (!fs.eof()) {
    std::string line;
    reg_t reg_before, reg_after = {0};
    inst_t instr = {};

    std::getline(fs, line);
    if (line.empty()) break;

    auto parse_from_file = [&](auto& before, auto& after, auto& inst) {
      std::sscanf(line.c_str(), "Before: [%d, %d, %d, %d]\n", &reg_before[0],
                  &reg_before[1], &reg_before[2], &reg_before[3]);
      std::getline(fs, line);
      std::sscanf(line.c_str(), "%d %d %d %d\n", &instr.op, &instr.a, &instr.b,
                  &instr.c);
      std::getline(fs, line);
      std::sscanf(line.c_str(), "After:  [%d, %d, %d, %d]", &reg_after[0],
                  &reg_after[1], &reg_after[2], &reg_after[3]);
      std::getline(fs, line);  // Skip blank line
    };

    parse_from_file(instr, reg_before, reg_after);
    if (matching_operations(instr, reg_before, reg_after) >= 3) res++;
  }
  std::cout << "Result part 1: " << res << '\n';
}

void get_unsolved_opcodes(const reg_t& before, const reg_t& after,
                          const inst_t inst,
                          std::array<dirty_op, 16>& unsolved_opcodes) 
{
  const std::array<op_t, 7> ops = {add, mult, band, bor, greater, equal, set};
  constexpr std::array ops_str = {"add",     "mult",  "band", "bor",
                                  "greater", "equal", "set"};

  auto idx = 0;
  for (const auto& op : ops) {
    auto (*const* op_ptr)(int, int) = op.target<int (*)(int, int)>();
    // Run all reg/reg operations
    if (after[inst.c] == op(before[inst.a], before[inst.b])) {
      std::string name_of_op = ops_str.at(idx);
      unsolved_opcodes.at(inst.op).op_number = inst.op;
      unsolved_opcodes.at(inst.op).op_occurences[name_of_op]++;
    }
    // Run all reg/immediate operations
    if (*op_ptr != set && after[inst.c] == op(before[inst.a], inst.b)) {
      std::string name_of_op = ops_str.at(idx) + std::string("_ri");
      unsolved_opcodes.at(inst.op).op_number = inst.op;
      unsolved_opcodes.at(inst.op).op_occurences[name_of_op]++;
    }
    // Run all immediate/reg operations
    if ((*op_ptr == set || *op_ptr == equal || *op_ptr == greater) &&
        after[inst.c] == op(inst.a, before[inst.b])) {
      std::string name_of_op = ops_str.at(idx) + std::string("_ir");
      unsolved_opcodes.at(inst.op).op_number = inst.op;
      unsolved_opcodes.at(inst.op).op_occurences[name_of_op]++;
    }
    idx++;
  }
}

auto figure_out_opcode(std::array<dirty_op, 16>& unsolved_ops) 
{
  std::array<std::string, 16> solved_ops = {};

  auto figure_out = [](auto& unsolved_ops, auto& solved_ops) {
    for (auto& [op_num, op_occur] : unsolved_ops) 
      if (op_occur.size() == 1) 
        for (const auto& [op_name, num_of_occur] : op_occur) 
          solved_ops[op_num] = op_name;
  };

  auto remove_solved = [](auto& dirty_ops, const auto& solved_ops) {
    for (auto& [op_num, op_occur] : dirty_ops) 
      for (const auto& solved : solved_ops) 
        if (!solved.empty()) 
          op_occur.erase(solved);
  };

  for (auto i = 0; i < 16; i++) {
    figure_out(unsolved_ops, solved_ops);
    remove_solved(unsolved_ops, solved_ops);
  }
  return solved_ops;
}

auto str_to_func(const std::array<std::string, 16>& opcodes_str) 
{
  std::array<op_exec, 16> op_codes = {};
  std::unordered_map<std::string, op_t> convert_str_to_func = {
      {"add", add},         {"mult", mult},   {"band", band}, {"bor", bor},
      {"greater", greater}, {"equal", equal}, {"set", set}};

  auto parse_op = [&](auto ops_str) {
    op_exec tmp_op = {false, false, nullptr};
    tmp_op.a_is_immediate = ops_str.find("_ir") != std::string::npos;
    tmp_op.b_is_immediate = ops_str.find("_ri") != std::string::npos;

    std::string op_category = ops_str.substr(0, ops_str.find("_"));
    tmp_op.op = convert_str_to_func[op_category];
    return tmp_op;
  };

  auto op_num = 0;
  for (const auto& op_code_str : opcodes_str) {
    op_codes[op_num] = parse_op(op_code_str);
    op_num++;
  }
  return op_codes;
}

auto calculate(const inst_t inst, const reg_t& regs,
               const std::array<op_exec, 16>& op_codes) 
{
  auto tmp_reg = regs;
  auto op = op_codes[inst.op];
  auto A = op.a_is_immediate ? inst.a : tmp_reg[inst.a];
  auto B = op.b_is_immediate ? inst.b : tmp_reg[inst.b];
  tmp_reg[inst.c] = op.op(A, B);
  return tmp_reg;
}

auto part2() 
{
  std::array<dirty_op, 16> unsolved_opcodes = {};
  std::ifstream fs("input.txt");

  // Read the first half of input
  while (!fs.eof()) {
    reg_t reg_before, reg_after = {0};
    inst_t instr{};
    std::string line;

    std::getline(fs, line);
    if (line.empty()) break;

    auto parse_from_file = [&](auto& before, auto& after, auto& inst) {
      std::sscanf(line.c_str(), "Before: [%d, %d, %d, %d]\n", &before[0],
                  &before[1], &before[2], &before[3]);
      std::getline(fs, line);
      std::sscanf(line.c_str(), "%d %d %d %d\n", &inst.op, &inst.a, &inst.b,
                  &inst.c);
      std::getline(fs, line);
      std::sscanf(line.c_str(), "After:  [%d, %d, %d, %d]", &after[0],
                  &after[1], &after[2], &after[3]);
      std::getline(fs, line);  // Skip blank line
    };

    parse_from_file(reg_before, reg_after, instr);
    get_unsolved_opcodes(reg_before, reg_after, instr, unsolved_opcodes);
  }

  auto filter_opcodes = [](auto& unfilt_opcodes) {
    for (auto& [op_num, op_freq] : unfilt_opcodes) {
      auto [max_op_name, max_freq] = *std::max_element(op_freq.begin(), op_freq.end(),
                            [](auto a, auto b) { return a.second < b.second; });

      for (auto it = op_freq.begin(); it != op_freq.end(); it++) 
        if (it->second < max_freq) 
          it = op_freq.erase(it);
    }
  };

  filter_opcodes(unsolved_opcodes);
  auto solved_opcodes_str = figure_out_opcode(unsolved_opcodes);
  auto solved_opcodes = str_to_func(solved_opcodes_str);

  // Read the second half of the input
  reg_t registers = {};
  std::string line;
  while (std::getline(fs, line)) {
    if (line.empty()) continue;
    inst_t instr;
    std::sscanf(line.c_str(), "%d %d %d %d\n", &instr.op, &instr.a, &instr.b, &instr.c);
    registers = calculate(instr, registers, solved_opcodes);
  }
  std::cout << "Result part 2: " << registers[0] << '\n';
}

int main() 
{
  std::ios::sync_with_stdio(false);
  using namespace std::chrono;
  auto start = steady_clock::now();

  part1();
  part2();

  auto end = steady_clock::now();
  std::cout << "\nExecution time: "
            << duration_cast<duration<float>>(end - start).count() << "s\n";
}