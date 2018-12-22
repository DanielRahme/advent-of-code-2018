// day_4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

struct Guard {
  int ID;
  int tot_sleep;
  std::vector<int> mins = std::vector<int>(60, 0);
};

auto getRecords(std::string file_name)
{
  std::ifstream fs(file_name);
  std::string line = "";
  std::vector<std::string> vec;

  while (std::getline(fs, line)) {
    vec.push_back(line);
  }
  return vec;
}

std::pair<int,int> parse(const std::string& record)
{
  int ID, min, dummy;
  std::sscanf(record.c_str(), "[%d-%d-%d %d:%d] Guard #%d begins shift", &dummy, &dummy, &dummy, &dummy, &min, &ID);
  return {min, ID};
}

void minutesAsleep(std::vector<int>& mins, const int start, const int end)
{
  for (auto i = start; i < end; i++) {
    mins[i]++;
  }
}

auto parseRecords(std::vector<std::string>& records)
{
  const auto sleep_msg_size = 31;
  const auto wake_msg_size = 27;

  auto current_guard = 0;
  auto start_sleep = 0;
  auto end_sleep = 0;
  std::vector<Guard> sleep_list;  // TODO: Replace with unordered_map

  for (const auto& rec : records) {
    auto length = rec.size();

    if (length == sleep_msg_size) { // Fall asleep
      start_sleep = parse(rec).first;

    } else if (length == wake_msg_size) { // Wake-up
      end_sleep = parse(rec).first;
      auto duration = end_sleep - start_sleep;
      Guard guard = {current_guard, duration};
      minutesAsleep(guard.mins, start_sleep, end_sleep);
      sleep_list.push_back(guard);

    } else {  // Guard enty
      current_guard = parse(rec).second;
    }
  }
  return sleep_list;
}

auto organizeSchedule(const std::vector<Guard>& guards)
{
  std::unordered_map<int, Guard> ordered_guards;

  // Fill the unordered_map with all IDs but empty values
  for (const auto& g : guards) {
    Guard tmp = {g.ID, 0};
    ordered_guards[g.ID] = tmp;
  }

  for (const auto& [id, tot_sleep, mins] : guards) {
    ordered_guards.find(id)->second.tot_sleep += tot_sleep;
    auto guard_mins = ordered_guards.find(id)->second.mins;

    std::vector<int> res;
    std::transform(mins.begin(), mins.end(), guard_mins.begin(), 
                    std::back_inserter(res), std::plus<int>());

    ordered_guards.find(id)->second.mins = res;
  }
  return ordered_guards;
}

auto part1(std::unordered_map<int, Guard>& guards)
{
  auto sleepiest = 0;
  auto longest_min = 0;
  auto max_sleep = 0;

  for (const auto& [id, g] : guards) {
    if (g.tot_sleep > max_sleep) {
      sleepiest = id;
      max_sleep = g.tot_sleep;

      longest_min = std::distance(g.mins.begin(), 
                                  std::max_element(g.mins.begin(), g.mins.end(),
                                                  [](int a, int b){ return a < b;})
      );
    }
  }
  return longest_min * sleepiest;
}

auto part2(std::unordered_map<int, Guard>& guards)
{
  auto max_freq = 0;
  auto id_max_freq = 0;
  auto minute = 0;

  for (const auto& [id, g] : guards) {
    auto freq = std::max_element(g.mins.begin(), g.mins.end(),
                                        [](int a, int b){ return a < b;}
    );

    if (*freq > max_freq) {
      max_freq = *freq;
      id_max_freq = id;
      minute = std::distance(g.mins.begin(), freq);
    }
  }
  return minute * id_max_freq;
}

int main() 
{
  //auto records = getRecords("test_input.txt");
  auto records = getRecords("input.txt");
  std::sort(records.begin(), records.end());

  auto guards = parseRecords(records);
  auto guards_ordered = organizeSchedule(guards);

  auto res_pt1 = part1(guards_ordered);
  std::cout << "Result part 1: " << res_pt1 << std::endl;

  auto res_pt2 = part2(guards_ordered);
  std::cout << "Result part 2: " << res_pt2 << std::endl;
}