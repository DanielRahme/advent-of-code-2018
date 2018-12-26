// day_7.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <utility>

struct Step {
  std::vector<char> dependencies;
  std::vector<char> unlocks;
};

std::vector<std::string> readInput(std::string text_file) 
{
  std::ifstream fs(text_file);
  std::string line = "";
  std::vector<std::string> input;

  while (std::getline(fs, line)) {
    input.push_back(line);
  }
  return input;
}

template<typename T>
void print(const T& iterable, char separator = ' ')
{
  for (const auto& elem : iterable) {
    std::cout << elem << separator;
  }
  std::cout << std::endl;
}

std::pair<char, char> parse(const std::string& input) 
{
  char dependencies, unlocks;
  std::sscanf(input.c_str(), "Step %c must be finished before step %c can begin.", &dependencies, &unlocks);
  return {dependencies, unlocks};
}

void parseSteps(std::map<char, Step>& steps, const std::vector<std::string>& input)
{
  for (const auto& i : input) {
    auto [dependencies, unlocks] = parse(i);
    steps[unlocks].dependencies.push_back(dependencies);
    steps[dependencies].unlocks.push_back(unlocks);
  }
}

void sortSteps(std::map<char, Step>& steps) 
{
  for (auto& [letter, step] : steps) {
    std::sort(step.dependencies.begin(), step.dependencies.end());
    std::sort(step.unlocks.begin(), step.unlocks.end());
  }
}

auto part1(const std::map<char, Step>& steps_original)
{
  auto steps = steps_original;
  std::vector<char> step_sequence; // This is where the final result is stored

  while (steps.size()) {
    auto sorted_step_sequence = step_sequence;
    std::sort(sorted_step_sequence.begin(), sorted_step_sequence.end());

    // Erase the unlocked letters from the steps
    for (const auto& unlocked_letter : sorted_step_sequence) {
      steps.erase(unlocked_letter);
    }

    // Check for next letter to add to sequence
    for (const auto& [letter, step] : steps) {
      auto can_unlock = std::includes(sorted_step_sequence.begin(), sorted_step_sequence.end(),
                                      step.dependencies.begin(), step.dependencies.end());

      if (can_unlock) {
        step_sequence.push_back(letter);
        break;  // Note the break!
      }
    }
  }
  std::string res(step_sequence.begin(), step_sequence.end());
  return res;
}

auto part2(const std::map<char, Step>& steps_original)
{
  auto seconds = 0;  // This is the time clock [s]
  const auto workers = 5;
  auto steps = steps_original;
  std::vector<char> step_sequence; 
  std::map<char, int> queue; // Letter and timer

  while (steps.size() || queue.size()) {
    // Erase all queued letter from steps
    for (const auto& [queue_letter, t_queue] : queue) {
      steps.erase(queue_letter);
      if (t_queue == 0) {
        step_sequence.push_back(queue_letter); // Insert letter when timer is done
      }
    }

    // Erase queued letter if done and inserted into step_sequence
    for (const auto& unlocked_letter : step_sequence) {
      queue.erase(unlocked_letter);
    }

    // Sort the step-sequence to be used for std::includes
    auto sorted_step_sequence = step_sequence;
    std::sort(sorted_step_sequence.begin(), sorted_step_sequence.end());

    // Check for next letter to add to sequence
    for (const auto& [letter, step] : steps) {
      auto can_unlock = std::includes(sorted_step_sequence.begin(), sorted_step_sequence.end(),
                                      step.dependencies.begin(), step.dependencies.end());

      if (can_unlock && queue.size() < workers) {
        int time = (int)letter - 64 + 60; // The -64 is due to 'A' starts at 65
        queue[letter] = time;
      }
    }

    // Update the timers
    for (auto& [letter_queue, t_queue] : queue) {
      t_queue--;
    }

    seconds++;
  }

  return seconds - 1;
}


int main()
{
	auto input = readInput("input.txt");

  std::map<char, Step> steps;
  parseSteps(steps, input);
  sortSteps(steps);

  auto res_pt1 = part1(steps);
  std::cout << "Result part 1:  " << res_pt1 << std::endl;

  auto res_pt2 = part2(steps);
  std::cout << "Result part 2:  " << res_pt2 << std::endl;

  return 0;
}

