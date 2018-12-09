// Day 1 part 2 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int res = 0;
bool dupe_found = 0;
std::vector<int> seen_freq;

int parse(std::string input)
{
  char sign = input.at(0);

  if (sign == '+') {
    input = input.substr(1);
  }

  return std::stoi(input);
}

int result(int input)
{
  std::ifstream fs("input.txt");
  std::string line;

  while (std::getline(fs, line)) {
    input += parse(line);

    if (std::find(seen_freq.begin(), seen_freq.end(), input) != seen_freq.end()) {
      std::cout << "dupe: " << input << std::endl;
      dupe_found = 1;
      seen_freq.push_back(input);
      break;
    }

    seen_freq.push_back(input);
  }
  return input;
}

int main()
{
  int freq = 0;

  while (!dupe_found) {
    freq = result(freq);
  }

  std::cout << "result: " << freq << std::endl;
  std::cout << "repeated freq: " << seen_freq.back() << std::endl;

  std::getchar();
  return 0;
}