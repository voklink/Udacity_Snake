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
// Stuff for class FOOD 
// --------------------------------------------------------------------------


void Foods::addFoodItem(int x, int y)
{
    if (_countOfFood < _maxCountOfFood)
    {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 5000);

       Food myFood{};
       SDL_Point newPoint{};

       newPoint.x = x;
       newPoint.y = y;

       myFood.setLiftetime(dis(gen));
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

void Foods::updateFoodList() 
{
    myFUNC;
    for (auto it = _foodList.begin(); it != _foodList.end(); ) 
    {
        myDEBUG(it->getTimeSinceSpawn().count());
        myDEBUG(it->getLifetime());
        if (it->getTimeSinceSpawn().count()>it->getLifetime())
        {
            it = _foodList.erase(it);       // Using ERASE makes it safe to remove an item from the vector during the loop!
            _countOfFood--;
        } else {
            ++it;
        }
    }


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