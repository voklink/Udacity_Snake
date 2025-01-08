#include "food.h"
#include <iostream>
#include "SDL.h"

void Foods::addFoodItem(int x, int y)
{
    if (_countOfFood < _maxCountOfFood)
    {
       Food myFood{};
       SDL_Point newPoint{};

       newPoint.x = x;
       newPoint.y = y;

       myFood.setLiftetime(100);
       myFood.setValue(1);
       myFood.setGrowth(1);
       myFood.setCoord(newPoint);
       
       _foodList.emplace_back(myFood);
       _countOfFood = _foodList.size(); 
       _timeSinceLastFoodSpawn = 0;

       std::cout<< "    Added Food at " << x << "/" << y <<"\n";
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

// void Foods::printFoodList(const Foods &foods) 
// {
//     std::vector<Food> foodList = foods.getFoodList();
//     for (const auto &food : foodList) {
//         SDL_Point coord = food.getCoord();
//         std::cout << "Food at (" << coord.x << ", " << coord.y << "), "
//                   << "Age: " << food.getAge() << ", "
//                   << "Lifetime: " << food.getLiftetime() << ", "
//                   << "Value: " << food.getValue() << std::endl;
//     }
// }

void Foods::printFoodList()  
{
    for (auto &food : _foodList) 
    {
        SDL_Point coord = food.getCoord();
        std::cout << "Food at (" << coord.x << ", " << coord.y << "), "
                    << "Age: " << food.getAge() << ", "
                    << "Lifetime: " << food.getLiftetime() << ", "
                    << "Value: " << food.getValue() << std::endl;
    }
}