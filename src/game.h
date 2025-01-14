#ifndef GAME_H
#define GAME_H

#include <random>
#include <sys/types.h>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"




class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer *renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void togglePause();

 private:
  Snake snake;
  // SDL_Point food;
  Foods _foods;
  std::vector<SDL_Point> _occupiedList;
  bool _isPaused;
  bool _isHyper;


  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceNewFood();
  void updateOccupiedList();
  void printOccupiedList();
  void Update();
  
  // void threadSnakeIsHyper(bool *isHyper);

};



#endif