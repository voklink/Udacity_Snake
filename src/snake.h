#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

#define myDEBUG(X) std::cout << (#X) << "= " << (X) << std::endl;
#define myPRINT(X) std::cout << (X) << std::endl;
#define myPRINT2(X,Y) std::cout << (X) << "  " << (Y) << std::endl;
#define myFUNC std::cout << "\n-------" << __func__ << "-------" << std::endl;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  void GrowBody(const int growth);
  bool SnakeCell(int x, int y);
  void printBodyList();

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  int growing{0};
  int grid_width;
  int grid_height;
};

#endif