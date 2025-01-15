#include "game.h"
#include <iostream>
#include "SDL.h"
#include "food.h"
#include <thread>
#include <future>

void threadSnakeIsHyper(std::shared_ptr<std::promise<void>> promise, bool *isHyper)  
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    *isHyper = false;
    promise->set_value(); // Notify that the work is done
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      _foods(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) 
{
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

void Game::Update() {
  if (this->_isPaused) return;
  if (!snake.alive) return;
  myFUNC;

  snake.speed = _isHyper? 1.0f : 0.1f;
  snake.Update();
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  std::cout << "Snakehead is at "  << new_x << "/" << new_y << "\n";
  
  updateOccupiedList();
  int growth{0};
  int currentScore{0};
  bool isHyper{false};
  _foods.updateFoodList(_occupiedList, currentScore, growth, isHyper);
  if (growth != 0) {snake.GrowBody(growth);};
  score += currentScore;

  if (isHyper && !_isHyper) 
  {
    _isHyper |= isHyper;

    // Create a shared_ptr to a promise and get its future
    auto promise = std::make_shared<std::promise<void>>();
    std::future<void> future = promise->get_future();

    // Start the thread with the shared_ptr to the promise
    std::thread hyperTimer(threadSnakeIsHyper, promise, &_isHyper);
    hyperTimer.detach();;
  }

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

void Game::updateOccupiedList()
{ 
  _occupiedList = std::vector<SDL_Point>{};       // reset the occupiedList
  _occupiedList.emplace_back(SDL_Point{static_cast<int>(snake.head_x), static_cast<int>(snake.head_y)});
  _occupiedList.insert(_occupiedList.end(),snake.body.begin(), snake.body.end());
   
  std::vector<SDL_Point> foodCoordinates{_foods.getAllFoodCoordinates()};
  _occupiedList.insert(_occupiedList.end(),foodCoordinates.begin(), foodCoordinates.end());
  
  // printOccupiedList();
}

void Game::printOccupiedList()
{
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "All blocked points: " << std::endl;
    for (const auto& point : _occupiedList)
     {
        std::cout << point.x << ", " << point.y << std::endl;
    }
  std::cout << "---------------------------------------------" << std::endl;
}

