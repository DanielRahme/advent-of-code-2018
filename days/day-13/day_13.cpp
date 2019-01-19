// day_13.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <optional>

struct Point {
  int x, y;

  Point (int X, int Y) {
    x = X;
    y = Y;
  }

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Point& other) const {
    return !(*this == other);
  }

  bool operator<(const Point& rhs) const {
    return y != rhs.y ? y < rhs.y : x < rhs.x;
  }
};

// A hash is created for unordered_map to have Point as a key
namespace std {
  template<>
  struct hash<Point> {
    std::size_t operator()(const Point& p) const {
      return ((std::hash<int>()(p.x)) ^ (std::hash<int>()(p.y) << 1) >> 1);
    }
  };
}

struct Cart {
  char direction;
  Point pos;
  int intersections;
};

std::pair<std::vector<Cart>, std::unordered_map<Point, char>> inputFromFile(const std::string& file_name) 
{
  std::ifstream fs(file_name);
  std::string line = "";
  std::vector<Cart> carts;
  std::unordered_map<Point, char> track;
  int y = 0;

  while (std::getline(fs, line)) {
    for (auto x = 0; x < line.size(); x++) {
      if (line[x] == '<' || line[x] == '>' || line[x] == '^' || line[x] == 'v') {
        carts.push_back({line[x], {x,y}, 0});
      } else if (line[x] != ' ' && line[x] != '-' && line[x] != '|') {
        Point p{x, y};
        track[p] = line[x];
      }
    }
    y++;
  }
  return {carts, track};
}

auto updatePos(Cart& cart)
{
  if (cart.direction == '>') cart.pos.x++;
  else if (cart.direction == '<') cart.pos.x--;
  else if (cart.direction == 'v') cart.pos.y++;
  else if (cart.direction == '^') cart.pos.y--;
}

auto rotate(const char cart_dir, const bool turn_right)
{
  const std::unordered_map<char, char> rotate_right{{'>','v'}, {'v','<'},{'<','^'},{'^','>'}};
  const std::unordered_map<char, char> rotate_left{{'>','^'}, {'v','>'},{'<','v'},{'^','<'}};
  return turn_right ? rotate_right.at(cart_dir) : rotate_left.at(cart_dir);
}

auto updateDirection(Cart& cart, const char track_path)
{
  if (track_path == '/') {
    const bool turn_right = cart.direction == 'v' || cart.direction == '^';
    cart.direction = rotate(cart.direction, turn_right);
  } else if (track_path == '\\') {
    const bool turn_right = cart.direction == '>' || cart.direction == '<';
    cart.direction = rotate(cart.direction, turn_right);
  } else if (track_path == '+') {
    int turn_right = cart.intersections++ % 3;
    if (turn_right != 1) 
      cart.direction = rotate(cart.direction, turn_right);
  }
}

auto detectCollision(const Cart cart, const std::vector<Cart>& carts)
{
  for (const auto& other_cart : carts) 
    if (cart.pos == other_cart.pos && cart.direction != other_cart.direction)
      return true;

  return false;
}

std::optional<Point> step_part1(std::vector<Cart>& carts, const std::unordered_map<Point, char>& track)
{
  std::sort(carts.begin(), carts.end(), 
    [](auto a, auto b){ return a.pos < b.pos;});

  for (auto& cart : carts) {
    updatePos(cart);

    if (detectCollision(cart, carts)) 
      return cart.pos;

    if (track.find(cart.pos) != track.end()) 
      updateDirection(cart, track.at(cart.pos));
  }
  return std::nullopt;
}

std::optional<Point> step_part2(std::vector<Cart>& carts, const std::unordered_map<Point, char>& track)
{
  const Point out_of_bounds = {-1, -1};

  std::sort(carts.begin(), carts.end(), 
    [](auto a, auto b){ return a.pos < b.pos;});

  for (auto& cart : carts) {
    if (cart.pos == out_of_bounds) 
      continue;

    updatePos(cart);
    
    if (detectCollision(cart, carts)) {
      const auto collision_pos = cart.pos;
      for (auto& c : carts) 
        if (c.pos == collision_pos) 
          c.pos = out_of_bounds;
    }

    if (track.find(cart.pos) != track.end()) 
      updateDirection(cart, track.at(cart.pos));
  }

  auto remaining_carts = std::count_if(carts.begin(), carts.end(),
                          [&](const auto& cart) {return cart.pos != out_of_bounds;});

  if (remaining_carts == 1) {
    auto last_cart = *std::find_if(carts.begin(), carts.end(), 
      [&](const auto& cart) {return cart.pos != out_of_bounds;});
    return last_cart.pos;  
  }
  return std::nullopt;
}

int main() 
{
  std::ios::sync_with_stdio(false);
  using namespace std::chrono;
  auto start = steady_clock::now();

  auto [carts, track] = inputFromFile("input.txt");
  auto carts_pt2 = carts;

  auto has_collided = step_part1(carts, track);
  while (!has_collided.has_value()) {
    has_collided = step_part1(carts, track);
  }
  std::cout << "Result part 1: " << has_collided->x << ',' << has_collided->y << std::endl;

  auto last_cart = step_part2(carts_pt2, track);
  while (!last_cart.has_value()) {
    last_cart = step_part2(carts_pt2, track);
  }
  std::cout << "Result part 2: " << last_cart->x << ',' << last_cart->y << std::endl;

  auto end = steady_clock::now();
  std::cout << "\nExecution time: " << duration_cast<duration<float>>(end - start).count() << "s\n";
}
