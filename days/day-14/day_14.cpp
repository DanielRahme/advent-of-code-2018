// day-14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <algorithm>
#include <sstream>

auto increment(uint64_t idx, const int recipe, const int size)
{
  idx += recipe + 1;
  while (idx >= size) 
    idx -= size;
  return idx;
}

auto tick(std::array<uint64_t, 2>& idx, std::vector<int>& rec)
{
  auto new_recipe = rec[idx[0]] + rec[idx[1]];
  if (new_recipe > 9) {
    rec.emplace_back(1);
    rec.emplace_back(new_recipe-10);
  } else {
    rec.emplace_back(new_recipe);
  }

  for (auto& i : idx) 
    i = increment(i, rec.at(i), rec.size());
}

auto getRecepie1(const int number)
{
  std::array<uint64_t, 2> eleves_idx{0, 1};
  std::vector<int> recipes{3, 7};

  while (recipes.size() < number + 10) 
    tick(eleves_idx, recipes);

  std::ostringstream res;
  std::copy(recipes.end()-10, recipes.end(), std::ostream_iterator<int>(res));
  return res.str();
}

auto correctSequence(const std::vector<int>& rec, const std::vector<int>& seq)
{
  if (rec.size() < seq.size()) 
    return false;
  std::vector<int> last_elements(rec.end()-seq.size(), rec.end());
  return last_elements == seq;
}

auto getRecepie2(std::string input)
{
  std::array<uint64_t, 2> eleves_idx{0, 1};
  std::vector<int> recipes{3, 7};
  recipes.reserve(20'000'000);
  std::vector<int> seq;
  
  for (auto i = 0; i < input.size(); i++) 
    seq.push_back(input[i] - '0');
  if (seq.back() == 1) seq.push_back(0); // Hack to avoid the skipped check when insertion of sum >= 10
  
  while (!correctSequence(recipes, seq)) 
    tick(eleves_idx, recipes);
  return recipes.size() - seq.size();
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