// day-9.cpp : Day 9 part 1 and 2
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

void printMarblesCurrent(std::vector<int> &v, int current)
{
  for (auto &i : v) {
    if (i == current)
      std::cout << "(" << i << ") ";
    else
      std::cout << i << " ";
  }
  std::cout << std::endl;
}


auto getScore(const int num_players, const uint32_t last_marble)
{
  std::vector<uint64_t> score(num_players);

  uint64_t next_marble_value = 0;
  int current_player = 0;
  uint64_t current_pos = 0;

  std::vector<int> marbles;

  marbles.push_back(0);
  marbles.push_back(1);

  next_marble_value = 2;
  current_player = 1;
  current_pos = 1;

  while (next_marble_value <= last_marble) {

    if (next_marble_value % 23 == 0) {
      score[current_player] += next_marble_value;       // Add multiple of 23

      int offset = (current_pos - 7 + marbles.size()) % marbles.size();   // Marble 7 ccw from current marble
      score[current_player] += marbles.at(offset);  // Add 7 ccw marble

      current_pos = offset;
      marbles.erase(std::find(marbles.begin(), marbles.end(), marbles.at(offset))); // Remove the 7 ccw marble
    }
    else {
      int insert_pos = (current_pos + 2) % marbles.size();

      if (insert_pos == 0) {
        marbles.push_back(next_marble_value);
        current_pos = marbles.size() - 1;
      }
      else {
        marbles.insert(marbles.begin() + insert_pos, next_marble_value);
        current_pos = insert_pos;
      }
    }

    next_marble_value++;
    current_player = (current_player + 1) % num_players;
  }
  return score;
}

auto maxScore(std::vector<uint64_t> v)
{
  return *std::max_element(std::begin(v), std::end(v));
}

int main()
{
  const int num_of_players = 400;
  const uint32_t last_marble = 7186400; // 71864 for part 1

  auto scores = getScore(num_of_players, last_marble);

  std::cout << "Highscore: " << maxScore(scores) << std::endl;

	std::getchar();
  return 0;
}

