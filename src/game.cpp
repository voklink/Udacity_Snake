#include "game.h"
#include <iostream>
#include "SDL.h"
#include "food.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceNewFood();
  PlaceNewFood();
  PlaceNewFood();
  PlaceNewFood();
}

void Game::Run(Controller const &controller, Renderer *renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;

  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, *this);
    Update();
    renderer->Render(snake, _foods);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer->UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceNewFood() 
{
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake.SnakeCell(x, y)) &&
        (_foods.checkPositionForFood(x,y) == -1))
    {
      _foods.addFoodItem(x,y);
      return;
    }
  }
}

// void Game::UpdateFood() 
// {

// }

void Game::Update() {
  if (this->_isPaused) return;
  if (!snake.alive) return;
  myFUNC;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  std::cout << "Snake is at "  << new_x << "/" << new_y << "\n";
  // Check if there's food over here
  int indexWithFood = _foods.checkPositionForFood(new_x, new_y);

  _foods.updateFoodList();
  PlaceNewFood();

  if (indexWithFood != -1) 
  {
    myPRINT("Scored!!!")

    score++;
    _foods.removeFood(indexWithFood);
    PlaceNewFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    // snake.speed += 0.02;
  }
  else {
    std::cout << "No food found at (" << new_x << ", " << new_y << ")" << std::endl;
  }

  // myPRINT("FoodList after Update:")
  _foods.printFoodList();
  myPRINT("================================================")
  return;
}

// Toggle the Pause status
void Game::togglePause()
{
  this->_isPaused = this->_isPaused ? false : true;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }