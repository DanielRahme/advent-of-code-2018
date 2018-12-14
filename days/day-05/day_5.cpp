// day_5.cpp : Advent of Code day 1 part 1 & 2
//

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


bool hasCollided(std::string& s, int idx)
{
  return (toupper(s[idx]) == toupper(s[idx + 1])
              && s[idx] != s[idx + 1]);
}


// Go through all particles once and remove collisions
// Return position of first collision occurance, return -1 if none occured
auto collideOnce(std::string& s, int start)
{
  auto first_collision_at = -1;
  auto has_collided_once = false;

  for (auto i = start; i < (s.size() - 1); i++) {
    if (!hasCollided(s, i))
      continue;
    
    // Remove the collided particles, i.e. aA, Cc, qQ, dD
    s.erase(i, 2);  

    if (!has_collided_once) {
      has_collided_once = true;
      first_collision_at = i-1;
    }
  }

  return first_collision_at;
}


// Cycle through the particles until there are no more collisions
auto collideAll(std::string& str)
{
  for (auto collide_at = 0; collide_at != -1; collide_at = collideOnce(str, collide_at));
}


void part1(std::string str)
{
  collideAll(str);
  std::cout << "Pt1 result: size = " << str.size() << std::endl;
}


void part2(std::string str)
{
  auto min_size = str.size();
  std::string alphabet = "abcdefghijklmnopqrstuvxyz";

  for (auto letter : alphabet) {
    auto tmp = str;
    tmp.erase(std::remove(tmp.begin(), tmp.end(), letter), tmp.end());
    tmp.erase(std::remove(tmp.begin(), tmp.end(), toupper(letter)), tmp.end()); // Erase upper case letter
    collideAll(tmp);

    if (min_size > tmp.size()) 
      min_size = tmp.size();
  }

  std::cout << "Pt2 result: min size = " << min_size << std::endl;
}


int main()
{
  // Read from input.txt and store in str.
  std::ifstream t("input.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string str = buffer.str();
  str = str.erase(str.size() - 1); // Remove endline character

  part1(str);
  part2(str);

  return 0;
}

