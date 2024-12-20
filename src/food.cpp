#include "food.h"
#include <iostream>
#include "SDL.h"

void Foods::addFoodItem(int x, int y)
{
    if (_countOfFood <= _maxCountOfFood)
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
    } 
}

// Food* Foods::checkPositionForFood(const int x,const int y)
// {
//     for (auto &food : _foodList) 
//     {
//         if (food.getCoord().x == x && food.getCoord().y == y) 
//         {
//             return &food;
//         }
//     }
//     return nullptr; // Wenn kein Food-Objekt an den Koordinaten gefunden wird
// }

std::vector<Food>::iterator Foods::checkPositionForFood(int x, int y)
 {
    for (auto it = _foodList.begin(); it != _foodList.end(); ++it) 
    {
        if (it->getCoord().x == x && it->getCoord().y == y)
        {
            return it;
        }
    }
    return _foodList.end(); // Wenn kein Food-Objekt an den Koordinaten gefunden wird
}

void Foods::removeFood(std::vector<Food>::iterator it)
{
    if (it != _foodList.end()) 
    {
        _foodList.erase(it);
    }
}