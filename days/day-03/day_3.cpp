// day_3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <set>

struct Point {
  int x;
  int y;
};

struct Claim {
  int ID;
  Point offset;
  Point rect;
  bool overlap = false;

  void print() {
    std::cout << "ID: " << ID << " ";
    std::cout << "Offset: " << offset.x << "," << offset.y << " ";
    std::cout << "Rect: " << rect.x << "," << rect.y << std::endl;
  }
};


auto printFabric(std::vector<std::vector<int>>& v)
{
  for (auto &row : v) {
    for (auto &col : row) {
      std::cout << col << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


auto printAllClaims(std::vector<Claim>& v)
{
  for (auto &it : v) {
    it.print();
  }
}


Claim parseClaim(std::string& claim_str)
{
  int id, x, y, w, h;
  std::sscanf(claim_str.c_str(), "#%d @ %d,%d: %dx%d\n", &id, &x, &y, &w, &h);
  return {id, {x, y}, {w, h}};
}

auto getClaimsFromFile(std::string file_name)
{
  std::ifstream fs(file_name);
  std::string line = "";
  std::vector<Claim> claims;

  while (std::getline(fs, line)) {
    claims.push_back(parseClaim(line));
  }
  return claims;
}


auto claimFabric(std::vector<std::vector<int>>& fabric, Claim& c, int overlap_symbol, std::set<int>& overlaps)
{
  for (auto i = c.offset.y; i < c.offset.y + c.rect.y; i++) {
    for (auto j = c.offset.x; j < c.offset.x + c.rect.x; j++) {
      if (fabric[i][j] > 0) {
        overlaps.insert(fabric[i][j]);
        overlaps.insert(c.ID);
        fabric[i][j] = overlap_symbol;
      }
      else if (fabric[i][j] != -1) {
        fabric[i][j] = c.ID;
      }
      else {
        overlaps.insert(c.ID);
      }
    }
  }

  return c;
}


auto countOverlaps(std::vector<std::vector<int>>& v, int overlap_num)
{
  return std::accumulate(v.begin(), v.end(), 0, // Sum of all rows
    [=](auto sum, const auto& row) {            // Predicate for accumulate
    return sum + std::count_if(row.begin(), row.end(),  // Count occurences in a row (1D vector)
      [=](auto i) { return i == overlap_num; });  // Preciate for count_if
  });
}

auto findNoOverlap(std::vector<Claim> claims, std::set<int> overlaps)
{
  std::set<int> all_id;
  for (auto& c : claims) {
    all_id.insert(c.ID);
  }

  std::set<int> diff;
  std::set_difference(all_id.begin(), all_id.end(), overlaps.begin(), overlaps.end(),
    std::inserter(diff, diff.begin()));

  auto res = 0;
  for (auto& i : diff) {
    res = i;
  }

  return res;
}

int main()
{
  const auto overlap_num = -1;
  const auto x_max = 1000;
  const auto y_max = 1000;

  std::vector<std::vector<int>> fabric(x_max, std::vector<int>(y_max, 0));
  std::vector<Claim> claims = getClaimsFromFile("input.txt");
  std::set<int> overlaps;

  for (auto &claim : claims) {
    claimFabric(fabric, claim, overlap_num, overlaps);
  }

  auto pt1_res = countOverlaps(fabric, overlap_num);
  std::cout << "Part 1) Number of overlaps: " << pt1_res << std::endl;

  auto pt2_res = findNoOverlap(claims, overlaps);
  std::cout << "Part 2) ID with no overlaps: " << pt2_res << std::endl;
}

