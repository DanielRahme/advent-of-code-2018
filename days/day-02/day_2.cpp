// aoc_day2.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <utility>
#include <vector>

std::pair<bool, bool> checkDoubleTripple(const std::string& line) 
{
  bool found_double = false;
  bool found_triple = false;

  std::set<char> unique_letters(line.begin(), line.end());
  if (unique_letters.size() == line.size()) 
    return  {false, false};

  for (const auto& c : line) {
    int count = std::count(line.begin(), line.end(), c);
    if (count == 2) found_double = true;
    if (count == 3) found_triple = true;
  }
  return  {found_double, found_triple};
}

int part1() 
{
  int doubles = 0;
  int triples = 0;

  std::ifstream fs("input.txt");
  std::string line;

  while (std::getline(fs, line)) {
    auto [found_double, found_triple] = checkDoubleTripple(line);
    if (found_double) doubles++;
    if (found_triple) triples++;
  }
  return doubles * triples;
}

auto getDiff(const std::string& s1, const std::string& s2)
{
  int diff = 0;
  for (int i = 0; i < s1.size(); i++) 
    diff += (s1[i] != s2[i]);
  return diff;
}

auto commonLetters(const std::string& s1, const std::string& s2)
{
  std::string result;
  for (auto i = 0; i < s1.size(); i++) {
    if (s1[i] == s2[i])
      result.push_back(s1[i]);
  }
  return result;
}

auto part2() 
{
  std::ifstream fs("input.txt");
  std::string line = "";
  std::vector<std::string> input;
  while (std::getline(fs, line)) 
    input.push_back(line);

  for (const auto& current_string : input) {
    for (const auto& other_string : input) {
      if (getDiff(current_string, other_string) == 1)
        return commonLetters(current_string, other_string);
    }
  }
  return std::string("Error: Match not found!");
}

int main() 
{
  int res_1 = part1();
  std::cout << "Result part 1: " << res_1 << std::endl;

  auto res_2 = part2();
  std::cout << "Result part 2: " << res_2 << std::endl;
}
