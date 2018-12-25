// day_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
#include <limits>
#include <set>
#include <numeric>

struct Point {
  int x;
  int y;
  int id;
};

auto inputFromFile(const std::string& file_name) 
{
  std::ifstream fs(file_name);
  std::string line = "";
  std::vector<std::string> vec;

  while (std::getline(fs, line)) {
    vec.push_back(line);
  }
  return vec;
}

void parse(const std::vector<std::string>& input, std::vector<Point>& vec) 
{
  for (auto i = 0; i < input.size(); i++) {
    int x, y;
    std::sscanf(input.at(i).c_str(), "%d, %d", &x, &y);
    vec.push_back({x, y, i+1});
  }
}

auto manhattan(const Point a, const Point b)
{
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

auto fillArea(std::vector<std::vector<int>>& area, const std::vector<Point>& coords)
{
  for (auto y = 0; y < 1000; y++) {
    for (auto x = 0; x < 1000; x++) {
      Point p_current = {x, y, 0};
      auto shortest_dist = std::numeric_limits<int>::max();

      for (const auto& c : coords) {
        auto dist = manhattan(p_current, c);
        if (dist < shortest_dist) {
          shortest_dist = dist;
          p_current.id = c.id;
        }
      }

      // Double check 
      for (const auto& c : coords) {
        auto dist = manhattan(p_current, c);
        if (dist <= shortest_dist && c.id != p_current.id) {
          p_current.id = -1;
        }
      }

      area[x][y] = p_current.id;
    }
  }
}


auto countLocations(const std::vector<std::vector<int>>& area, const std::vector<Point> coord)
{
  std::vector<std::pair<Point, int>> locations;
  for (const auto& p : coord) {
    const auto id_to_check = p.id;

    auto sum = std::accumulate(area.begin(), area.end(), 0, 
          [=](auto sum, auto row) {
            return sum + std::count(row.begin(), row.end(), id_to_check);
        });
    locations.emplace_back(p, sum);
  }
  return locations;
}

auto findInfinite(const std::vector<std::vector<int>>& area)
{
  std::set<int> infinite;
  // Check top and bottom side
  for (auto x = 0; x < 1000; x++) {
    infinite.insert(area[x][0]);
    infinite.insert(area[x][999]);
  }

  // Check left and right side
  for (auto y = 0; y < 1000; y++) {
    infinite.insert(area[0][y]);
    infinite.insert(area[999][y]);
  }
  return infinite;
}

auto findMax(const std::vector<std::pair<Point, int>> locations, const std::set<int>& infinite)
{
  std::vector<int> sum_list;
  for (const auto& [p, sum] : locations) {
    auto in_blacklist = infinite.find(p.id) != infinite.end();
    if (!in_blacklist) {
      sum_list.push_back(sum);
    }
  }
  auto max = *std::max_element(sum_list.begin(), sum_list.end());
  return max;
}

auto part1(const std::vector<Point>& coord)
{
  std::vector<std::vector<int>> area(1000, std::vector<int>(1000, 0)); // int area[1000][1000] = {0}
  fillArea(area, coord);

  auto locations = countLocations(area, coord);
  auto infinite = findInfinite(area);
  return findMax(locations, infinite);
}

auto part2(const std::vector<Point>& coords)
{
  const auto limit = 10000;
  auto under_limit_count = 0;

  for (auto y = 0; y < 1000; y++) {
    for (auto x = 0; x < 1000; x++) {
      const Point current_point = {x, y, 0};
      auto sum = std::accumulate(coords.begin(), coords.end(), 0,
                    [=](auto sum, auto c){return sum + manhattan(current_point, c);}
                  );

      if (sum < limit) {
        under_limit_count++;
      }
    }
  }
  return under_limit_count;
}

int main() 
{
  auto input = inputFromFile("input.txt");

  std::vector<Point> coordinates;
  parse(input, coordinates);

  auto res_pt1 = part1(coordinates);
  std::cout << "Part 1 result: " << res_pt1 << std::endl;

  auto res_pt2 = part2(coordinates);
  std::cout << "Part 2 result: " << res_pt2 << std::endl;
}
