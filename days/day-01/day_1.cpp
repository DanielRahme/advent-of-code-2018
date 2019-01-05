// day_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <chrono>

auto part1()
{
  auto frequency = 0;
  std::ifstream fs("input.txt");
  while (fs.good()) {
    auto change = 0;
    fs >> change;
    frequency += change;
  }
  return frequency;
}

auto part2()
{
  auto frequency = 0;
  std::vector<int> changes;
  std::unordered_map<int,int> seen_frequencies;
  std::ifstream fs("input.txt");

  while (fs.good()) {
    auto change = 0;
    fs >> change;
    changes.push_back(change);
  }

  while (true) {
    for (const auto& c : changes) {
      frequency += c;
      seen_frequencies[frequency]++;
      if (seen_frequencies[frequency] > 2)
        return frequency;
    }
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  using namespace std::chrono;
  auto start = steady_clock::now();

  auto sum_part1 = part1();
  std::cout << "Result part 1: " << sum_part1 << std::endl;; 

  auto sum_part2 = part2();
  std::cout << "Result part 2: " << sum_part2 << std::endl;; 

  auto end = steady_clock::now();
  std::cout << "\nExecution time: "
            << duration_cast<duration<float>>(end - start).count() << "s\n";
}
