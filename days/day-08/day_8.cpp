// day_8.cpp : This file contains the 'main' function. Program execution begins
// and ends there.
//

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Node {
  std::vector<int> meta_data;
  std::vector<Node> children;

  void addChild(Node child) {
    children.push_back(child); 
  }
};

Node getChildNode(std::ifstream& fs) 
{
  Node child;
  auto num_children = 0;
  auto num_meta = 0;
  fs >> num_children >> num_meta;

  for (auto i = 0; i < num_children; i++) {
    auto tmp = getChildNode(fs);
    child.addChild(tmp);
  }

  for (auto i = 0; i < num_meta; i++) {
    auto meta = 0;
    fs >> meta;
    child.meta_data.push_back(meta);
  }
  return child;
}

void buildTree(Node& root, std::ifstream& fs) 
{
  auto num_children = 0;
  auto num_meta = 0;
  fs >> num_children >> num_meta;

  for (auto i = 0; i < num_children; i++) {
    auto child = getChildNode(fs);
    root.addChild(child);
  }

  for (auto i = 0; i < num_meta; i++) {
    auto meta = 0;
    fs >> meta;
    root.meta_data.push_back(meta);
  }
}

int part1(const Node& node, int sum) 
{
  // The sum of the meta data
  sum = std::accumulate(node.meta_data.begin(), node.meta_data.end(), sum);

  // The sum of meta data from the child nodes
  for (const auto& i : node.children) {
    sum = part1(i, sum);
  }
  return sum;
}

int part2(const Node& node, int sum) 
{
  // Return sum of meta if no child nodes exist
  if (node.children.empty())
    return std::accumulate(node.meta_data.begin(), node.meta_data.end(), sum);

  // Get meta data from child node of index
  for (auto& idx : node.meta_data) {
    if (idx != 0 && idx <= node.children.size()) {
      sum = part2(node.children[idx - 1], sum);
    }
  }
  return sum;
}

int main() 
{
  std::ifstream fs("input.txt");
  Node root;

  buildTree(root, fs);

  auto res_pt1 = part1(root, 0);
  std::cout << "Result part 1: " << res_pt1 << std::endl;

  auto res_pt2 = part2(root, 0);
  std::cout << "Result part 2: " << res_pt2 << std::endl;
}
