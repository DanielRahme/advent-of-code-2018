// day_11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <string>

struct Point {
  int x;
  int y;
};

auto power(const int x, const int y, const int grid_serial)
{
  auto power = (x + 10) * (x*y + 10*y + grid_serial);
  return (power % 1000) / 100 - 5;
}

auto fillGrid(const int serial, std::array<int, 300*300>& grid)
{
  for (auto y = 0; y < 300; y++) {
    for (auto x = 0; x < 300; x++) {
      grid[y*300 + x] = power(x,y, serial);
    }
  }
}

auto totalPower(const Point start, const int square_size, const std::array<int,300*300>& grid)
{
  auto sum = 0;
  for (auto y = start.y; y < start.y + square_size; y++) {
    for (auto x = start.x; x < start.x + square_size; x++) {
      sum += grid[y*300 + x];
    }
  }
  return sum;
}

auto findMaxPower(const std::array<int,300*300>& grid, const int square_size)
{
  auto max_total_power = 0;
  Point p_max = {0,0};

  for (auto y = 0; y < 300 - square_size; y++) {
    for (auto x = 0; x < 300 - square_size; x++) {
      auto total_power = totalPower({x,y}, square_size, grid);
      if (max_total_power < total_power) {
        max_total_power = total_power;
        p_max = {x, y};
      }
    }
  }
  return p_max;
}

auto part2(const std::array<int,300*300>& grid) 
{
  auto max_sum = 0;
  auto max_sqr_size = 0;
  Point max_point = {0,0};

  for (auto sqr_size = 3; sqr_size <= 30; sqr_size++) {
    auto current_max_point = findMaxPower(grid, sqr_size);
    auto sum = totalPower(current_max_point, sqr_size, grid);

    if (sum > max_sum) {
      max_point = current_max_point;
      max_sum = sum;
      max_sqr_size = sqr_size;
    }
  }

  std::cout << "Part 2 result: " << max_point.x << ',' << max_point.y << ',' << max_sqr_size << std::endl;
}

int main() 
{ 
  const auto grid_serial = 3214; // puzzle input
  std::array<int, 300*300> grid{0};

  fillGrid(grid_serial, grid);
  auto [x,y] = findMaxPower(grid, 3);
  std::cout << "Part 1 result: " << x << ',' << y << std::endl;

  part2(grid);
}
