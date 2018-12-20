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

struct Claim {
  int ID;
  int x;
  int y;
  int width;
  int height;
};

Claim parseClaim(std::string &claim_str)
{
  int id, x, y, w, h;
  std::sscanf(claim_str.c_str(), "#%d @ %d,%d: %dx%d\n", &id, &x, &y, &w, &h);
  return {id, x, y, w, h};
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

auto removeFromVec(std::vector<int> &vec, int element)
{
  vec.erase(std::remove(vec.begin(), vec.end(), element), vec.end());
}

// Calculate the number of overlapped claimes. Modify list of ID by deleting overlapping IDs.
// Return number of overlaps.
auto calcOverlaps(const std::vector<Claim> &claims, std::vector<int> &no_overlap_id, const int overlap_symbol)
{
  auto overlaps = 0;
  std::vector<std::vector<int>> fabric(1000, std::vector<int>(1000, 0)); // int fabric[1000][1000] = {0}

  for (const auto &c : claims) {
    for (auto i = c.y; i < c.y + c.height; i++) {
      for (auto j = c.x; j < c.x + c.width; j++) {
        // Unclaimed area
        if (fabric[i][j] == 0) {
          fabric[i][j] = c.ID;
        } else {
          removeFromVec(no_overlap_id, c.ID);
          removeFromVec(no_overlap_id, fabric[i][j]);

          // Area overlaped first time. Done to prevent counting duplicates
          if (fabric[i][j] != -1) {
            overlaps++;
            fabric[i][j] = overlap_symbol;
          }
        }
      }
    }
  }

  return overlaps;
}

int main()
{
  const auto overlap_num = -1;

  std::vector<Claim> claims = getClaimsFromFile("input.txt");
  std::vector<int> no_overlap_id;

  // Fill no_overlap_id with all ID's from claims
  std::transform(claims.begin(), claims.end(), std::back_inserter(no_overlap_id),
                 [](auto x) { return x.ID; });

  auto res_pt1 = calcOverlaps(claims, no_overlap_id, overlap_num);
  std::cout << "Part 1) Number of overlaps: " << res_pt1 << std::endl;

  auto res_pt2 = *no_overlap_id.begin();
  std::cout << "Part 2) ID with no overlaps: " << res_pt2 << std::endl;
}
