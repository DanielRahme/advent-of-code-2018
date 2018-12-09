// day_5.cpp : Advent of Code day 1 part 1 
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <streambuf>
#include <sstream>

void collision(std::string& str)
{
  std::cout << "start collision" << std::endl;
  for (uint64_t i = 0; i < str.size() - 1; i++) {
    char current = str[i];
    char next = str[i + 1];

    if (toupper(current) == toupper(next)
        && isupper(current) ^ isupper(next)) 
    {

      str = str.erase(i, 2);
      i = 0;
    }
  }
}

int main()
{
  // Read from input.txt and store in str.
  std::ifstream t("input.txt");
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string str = buffer.str();

  str = str.erase(str.size() - 1); // Remove endline character

  collision(str);

  std::cout << "size: " << str.size() << std::endl;

  std::getchar();
  return 0;
}

