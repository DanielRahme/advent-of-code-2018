// day_12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>

auto parseInput(std::string text_file, std::vector<bool>& pots) 
{
  std::ifstream fs(text_file);
  std::string line = "";
  std::vector<std::vector<bool>> combinations;

  std::getline(fs, line); // Get the first row with initial value
  line.erase(0, 15);
  for (const auto& current_char : line) 
    pots.push_back(current_char == '#');

  std::getline(fs, line); // Skip blank row
  // Get the combinations that generate a plant
  while (std::getline(fs, line)) {
    if (line.back() == '#') {
      std::vector<bool> combo;
      for (auto i = 0; i < 5; i++) 
        combo.push_back(line[i] == '#');
      combinations.push_back(combo);
    }
  }
  return combinations;
}

auto isNewPlant(const std::vector<bool>& five_pots, const std::vector<std::vector<bool>>& combos)
{
  for (const auto& combo : combos) {
    if (combo == five_pots)  
      return true;
  }
  return false;
}

auto evolve(std::vector<bool>& pots, const std::vector<std::vector<bool>>& combos)
{
  static auto origin_dist = 0;

  // Insert leading zeroes (always 3)
  auto it_first_plant = std::find(pots.begin(), pots.end(), 1);
  auto leftmost_plant = std::distance(pots.begin(), it_first_plant);
  for (auto i = 0; i < 3 - leftmost_plant; i++) {
    pots.insert(pots.begin(), 0);
    origin_dist--;
  }
  // Append trailing zeroes (always 3)
  auto it_last_plant = std::find(pots.rbegin(), pots.rend(), 1);
  auto rightmost_plant = pots.size() - std::distance(it_last_plant, pots.rend());
  for (auto i = 0; i < 3-rightmost_plant; i++)
    pots.push_back(0);

  // Evolve the plants!
  std::vector<bool> new_pots;
  for (auto it = pots.begin()+2; it != pots.end()-2; it++) {
    std::vector<bool> five_pots(it-2, it+3);
    new_pots.push_back(isNewPlant(five_pots, combos));
  }
  pots = std::move(new_pots);
  origin_dist += 2; // When evolving, the 2 leftmost zeroes are gonski
  return origin_dist;
}

auto part1(std::vector<bool>& pots, const std::vector<std::vector<bool>>& combinations)
{
  unsigned long long leftmost_idx = 0;
  for (auto i = 0; i < 20; i++) 
    leftmost_idx = evolve(pots, combinations);

  auto sum_pt1 = std::accumulate(pots.begin(), pots.end(), 0ull,
                  [&](const auto& sum, const auto& pot){ return sum + (pot * leftmost_idx++);});

  return sum_pt1;
}

auto part2(std::vector<bool>& pots, const std::vector<std::vector<bool>>& combinations)
{
  unsigned long long leftmost_idx = 0;
  for (auto i = 0; i < 80; i++) 
    leftmost_idx = evolve(pots, combinations);

  leftmost_idx += 50'000'000'000 - 100;
  auto sum_pt2 = std::accumulate(pots.begin(), pots.end(), 0ull,
                  [&](const auto& sum, const auto& pot){ return sum + (pot * leftmost_idx++);});

  return sum_pt2;
}

int main()
{
  std::ios::sync_with_stdio(false);
  using namespace std::chrono;
  auto start = steady_clock::now();
  
  std::vector<bool> pots;
  auto combinations = parseInput("input.txt", pots);

  auto sum_pt1 = part1(pots, combinations);
  std::cout << "The sum pt1: " << sum_pt1 << std::endl; 

  auto sum_pt2 = part2(pots, combinations);
  std::cout << "The sum pt2: " << sum_pt2 << std::endl; 

  auto end = steady_clock::now();
  std::cout << "\nExecution time: " << duration_cast<duration<float>>(end - start).count() << "s\n";
}
