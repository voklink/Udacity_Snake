#include "food.h"
#include <iostream>
#include <sys/types.h>
#include "SDL.h"
// --------------------------------------------------------------------------
// Stuff for class FOOD 
// --------------------------------------------------------------------------

// Constructor, which sets the spawn time
Food::Food() : _spawnTime(std::chrono::steady_clock::now()) {}

std::chrono::milliseconds Food::getTimeSinceSpawn() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _spawnTime);
}

void Food::update()
{
    setAge(getTimeSinceSpawn());
}



// --------------------------------------------------------------------------
// Class FOODS
// --------------------------------------------------------------------------

// Constructor with init list
Foods::Foods(int grid_width, int grid_height)
    : _grid_width(grid_width),
      _grid_height(grid_height),
      _timeSinceLastFoodSpawn(0),
      _gen(_rd()),
      _dis_width(0, grid_width - 1),
      _dis_height(0, grid_height - 1) {}


void Foods::addFoodItemAtPoint(int x, int y)
{
       Food myFood{};
       SDL_Point newPoint{x,y};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 5000);


       myFood.setLiftetime(dis(gen));
       myFood.setValue(1);
       myFood.setGrowth(1);
       myFood.setCoord(newPoint);
       
       _foodList.emplace_back(myFood);
       _countOfFood = _foodList.size(); 
       _timeSinceLastFoodSpawn = 0;

       std::cout<< "    Added Food at " << x << "/" << y <<"\n";
} 

void Foods::addNewFood(std::vector<SDL_Point>& occupiedList) 
{
    myFUNC;
        while (_countOfFood < _maxCountOfFood) 
        {
            int x = _dis_width(_gen);
            int y = _dis_height(_gen);
            
            myDEBUG(x);
            myDEBUG(y);
            bool pointIsOccupied{false};
            // Check that the location is not occupied by a snake item before placing
            // food.
            myPRINT("Checking points of OccList:")
            for (auto point : occupiedList)
            {
                myDEBUG(point.x);
                myDEBUG(point.y);
                if ((point.x == x) && (point.y == y))
                {
                    pointIsOccupied |= true;
                    break;                      // if one point is occupied by food or snake, we can stop checking the list and generate another random value
                }
            }

            if (!pointIsOccupied)
            {
                addFoodItemAtPoint(x,y);
                occupiedList.emplace_back(SDL_Point{x,y});
                return;
            }
        }
    }

int Foods::checkPositionForFood(const int x, const int y) 
{
    for (size_t i = 0; i < _foodList.size(); ++i) 
    {
        SDL_Point coord = _foodList[i].getCoord();
        std::cout << "Checking food at (" << coord.x << ", " << coord.y << ") against (" << x << ", " << y << ")" << std::endl;
        if (coord.x == x && coord.y == y) {
            std::cout << "Found food at " << x << "/" << y << "\n";
            return i;
        }
    }
    return -1; // If there's no food object at that location, return -1
}

void Foods::removeFood(const int index) 
{
    if (index >= 0 && index < _foodList.size())
    {
        std::cout << "Removing food at (" << _foodList[index].getCoord().x << ", " << _foodList[index].getCoord().y << ")" << std::endl;
        _foodList.erase(_foodList.begin() + index);
        --_countOfFood;
    } 
    else 
    {
        std::cout << "Invalid index, cannot remove" << std::endl;
    }
}

void Foods::updateFoodList(std::vector<SDL_Point>& occupiedList) 
{
    while (_countOfFood < _maxCountOfFood)
    {
        addNewFood(occupiedList);
    }

    for (auto it = _foodList.begin(); it != _foodList.end(); ) 
    {
        if (it->getTimeSinceSpawn().count()>it->getLifetime())
        {
            it = _foodList.erase(it);       // Using ERASE makes it safe to remove an item from the vector during the loop!
            _countOfFood--;
        } else {
            ++it;
        }
    }
}


std::vector<SDL_Point> Foods::getAllFoodCoordinates() const
{
    std::vector<SDL_Point> myPointList{};
    for (auto &food : _foodList)  
    {
        myPointList.emplace_back(food.getCoord());
    }
    return myPointList;
}

void Foods::printFoodList()  
{
    for (auto &food : _foodList) 
    {
        SDL_Point coord = food.getCoord();
        std::cout << "Food at (" << coord.x << ", " << coord.y << "), "
                    << "Age: " << (food.getTimeSinceSpawn().count()) << ", "
                    << "Lifetime: " << food.getLifetime() << ", "
                    << "Value: " << food.getValue() << std::endl;
    }
}