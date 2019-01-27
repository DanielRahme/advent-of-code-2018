// day-14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>

auto increment(int idx, const int recipe, const int size)
{
  idx += recipe + 1;
  while (idx >= size) 
    idx -= size;
  return idx;
}

auto tick(std::array<int, 2>& idx, std::vector<int>& rec)
{
  auto new_recipe = rec[idx[0]] + rec[idx[1]];
  if (new_recipe > 9) 
    rec.emplace_back(1);
  rec.emplace_back(new_recipe % 10);

  for (auto& i : idx) 
    i = increment(i, rec.at(i), rec.size());
}

auto getRecepie1(const int number)
{
  std::array<int, 2> eleves_idx{0, 1};
  std::vector<int> recipes{3, 7};

  while (recipes.size() < number + 10) 
    tick(eleves_idx, recipes);

  std::ostringstream res;
  std::copy(recipes.end()-10, recipes.end(), std::ostream_iterator<int>(res));
  return res.str();
}

auto getRecepie2(std::string input)
{
  std::array<int, 2> eleves_idx{0, 1};
  std::vector<int> recipes{3, 7};
  recipes.reserve(20'000'000);
  std::vector<int> seq;
  
  for (const auto& c : input) 
    seq.push_back(c - '0');
  
  auto correct_sequence = [&](){
    return std::includes(recipes.end() - seq.size() - 1, recipes.end(), seq.begin(), seq.end());};

  while (!correct_sequence()) 
    tick(eleves_idx, recipes);
  return recipes.size() - seq.size() - 1;
}

int main()
{
  std::ios::sync_with_stdio(false);
  using namespace std::chrono;
  auto start = steady_clock::now();

  const auto input = 323081;

  auto res_part_1 = getRecepie1(input);
  std::cout << "Result part 1: " << res_part_1 << std::endl;

  auto res_part_2 = getRecepie2(std::to_string(input));
  std::cout << "Result part 2: " << res_part_2 << std::endl;

  auto end = steady_clock::now();
  std::cout << "\nExecution time: " << duration_cast<duration<float>>(end - start).count() << "s\n";
}