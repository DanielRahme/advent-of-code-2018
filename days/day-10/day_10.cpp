// day_10.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using std::vector;

struct Point {
  int x;
  int y;
  int dx;
  int dy;
};

auto readInput(std::string text_file) 
{
  std::ifstream fs(text_file);
  std::string line = "";
  std::vector<Point> input;

  while (std::getline(fs, line)) {
    int x, y, dx, dy;
    std::sscanf(line.c_str(), "position=<%d,%d> velocity=<%d,%d>", &x, &y, &dx, &dy);
    input.push_back({x, y, dx, dy});
  }
  return input;
}

auto updatePoints(vector<Point>& points, const int steps = 1) 
{
  for (auto& p : points) {
    p.x += p.dx * steps;
    p.y += p.dy * steps;
  }
}

std::pair<Point, Point> findBoundry(const vector<Point>& points) 
{
  auto [it_min_x, it_max_x] = std::minmax_element( points.begin(), points.end(), 
                                [](const auto& a, const auto& b) {return a.x < b.x;});
  auto [it_min_y, it_max_y] = std::minmax_element( points.begin(), points.end(), 
                                [](const auto& a, const auto& b) {return a.y < b.y;});

  Point min = {it_min_x->x, it_min_y->y};
  Point max = {it_max_x->x, it_max_y->y};
  return {min, max};
}

auto printMatrix(const vector<Point>& points, const std::pair<Point,Point> boundry) 
{
  auto [min, max] = boundry;

  const auto point_match = [&](const auto& x, const auto& y) {
    auto it_found_point = find_if(points.begin(), points.end(), 
                  [&](const auto& p) {return p.x == x && p.y == y;});
    return it_found_point != points.end();
  };

  for (auto y = min.y; y <= max.y; y++) {
    for (auto x = min.x; x <= max.x; x++) {
      auto found_point = point_match(x, y);
      std::cout << (found_point ? '#' : '.') << ' ';
    }
    std::cout << '\n';
  }
}

int main() 
{
  auto step_count = 0;
  const auto skip_steps = 10000;
  const auto text_height = 10;
  auto points = readInput("input.txt");

  updatePoints(points, skip_steps);  // Skip first 10'000 steps
  auto boundry = findBoundry(points);

  while (std::abs(boundry.first.y - boundry.second.y) > text_height) {
    updatePoints(points);
    boundry = findBoundry(points);
    step_count++;
  }

  std::cout << "Result part 1:\n";
  printMatrix(points, boundry);
  std::cout << '\n' << "Result part 2, steps: " << step_count + skip_steps << '\n';
}
