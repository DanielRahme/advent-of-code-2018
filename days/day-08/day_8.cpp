// day_8.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Node {
  std::vector<int> metadata;
  std::vector<Node> children;
};

Node getChildNode(std::ifstream& fs) 
{
  Node node;
  auto num_children = 0;
  auto num_meta = 0;
  fs >> num_children >> num_meta;

  for (auto i = 0; i < num_children; i++) 
    node.children.push_back(getChildNode(fs));

  for (auto i = 0; i < num_meta; i++) {
    auto meta = 0;
    fs >> meta;
    node.metadata.push_back(meta);
  }
  return node;
}

// Sum of all metadata
int part1(const Node& node) 
{
  auto sum = std::accumulate(node.metadata.begin(), node.metadata.end(), 0);
  return std::accumulate(node.children.begin(), node.children.end(), sum, 
          [&](const auto& s, const auto& n){return s + part1(n);});
}

// Return sum of meta if no child nodes exist, else return sum of child index (metadata)
int part2(const Node& node) 
{
  if (node.children.empty())
    return std::accumulate(node.metadata.begin(), node.metadata.end(), 0);

  return std::accumulate(node.metadata.begin(), node.metadata.end(), 0,
          [&](auto s, auto meta){ return meta <= node.children.size() ? s + part2(node.children[meta-1]) : s;});
}

int main() 
{
  std::ifstream fs("input.txt");
  Node root = getChildNode(fs);

  auto res_pt1 = part1(root);
  std::cout << "Result part 1: " << res_pt1 << std::endl;

  auto res_pt2 = part2(root);
  std::cout << "Result part 2: " << res_pt2 << std::endl;
}