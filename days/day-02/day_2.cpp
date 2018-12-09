// day_2.cpp : Day 2 part 1 and 2 (partially)
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <tuple>
#include <vector>

std::string sortString(std::string word)
{
	std::sort(word.begin(), word.end());
	return word;
}

std::tuple<bool, bool> checkDoubleTripple(std::string sortedWord)
{
	bool found_double = false;
	bool found_triple = false;

	std::set<char> unique_letters(sortedWord.begin(), sortedWord.end());

	if (unique_letters.size() == sortedWord.size()) {
		return std::make_tuple(false, false);
	}


	for (int i = 0; i < sortedWord.size(); i++) {
		char letter = sortedWord.at(i);
		int count = std::count(sortedWord.begin(), sortedWord.end(), letter);

		if (count == 2)
			found_double = true;
		if (count == 3)
			found_triple = true;
	}

	return std::make_tuple(found_double, found_triple);
}

int part1()
{
	int doubles = 0;
	int triples = 0;

	std::ifstream fs("input.txt");
	std::string line;

	while (std::getline(fs, line)) {
		auto isDoubleTriple = checkDoubleTripple(line);

		if (std::get<0>(isDoubleTriple)) {
			doubles++;
		}
		if (std::get<1>(isDoubleTriple)) {
			triples++;
		}
	}

	return doubles * triples;
}

std::string::size_type diffLetters(std::string s1, std::string s2) {
	sort(begin(s1), end(s1));
	sort(begin(s2), end(s2));
	std::string intersection;
	std::set_intersection(begin(s1), end(s1), begin(s2), end(s2), back_inserter(intersection));
	return s1.size() - intersection.size();
}

int getDiff(std::string str1, std::string str2) 
{
	int diff = 0;

	if (str1.empty() || str2.empty()) {	// Check if empty
		return -1;
	}

	if (str1.size() != str2.size()) {	// Check if different length
		return -2;
	}
	
	for (int i = 0; i < str1.size(); i++) {
		if (str1.at(i) != str2.at(i)) {
			diff++;
		}
	}

	return diff;
}

void part2()
{
	std::ifstream fs("input.txt");
	std::string line = "";
	int diff = 0;
	std::vector<std::string> input;

	while (std::getline(fs, line)) {
		input.push_back(line);
	}

	for (std::vector<int>::size_type i = 0; i != input.size(); i++) {
		std::string current_string = input[i];

		for (std::vector<int>::size_type j = 0; j != input.size(); j++) {
			diff = getDiff(current_string, input[j]);
			if (diff == 1) {
				std::cout << "diff: " << diff << std::endl;
				std::cout << "str1: " << current_string << std::endl;
				std::cout << "str2: " << input[j] << std::endl << std::endl;
			}
		}
	}
}


int main()
{
	int res_1 = part1();
	std::cout << "result: " << res_1 << std::endl;

	part2();
  std::cout << "Pt2: Solve it yourself!: " << std::endl;

  std::getchar();
	return 0;
}
