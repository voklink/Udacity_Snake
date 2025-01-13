#include "food.h"
#include <iostream>
#include <sys/types.h>
#include "SDL.h"
// --------------------------------------------------------------------------
// class FOOD 
// --------------------------------------------------------------------------

// Constructor, which sets the spawn time and coordinates
Food::Food(int x, int y) : _spawnTime(std::chrono::steady_clock::now()),
                           _age(0),
                           _coordinates({x, y}) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2000, 10000);

    setLiftetime(dis(gen));
    setValue(1);
    setGrowth(1);
}

std::chrono::milliseconds Food::getTimeSinceSpawn() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _spawnTime);
}

void Food::update()
{
    myPRINT("Calling Food:Update");
    setAge(getTimeSinceSpawn().count());
}

// --------------------------------------------------------------------------
// Child class MovingFood
// --------------------------------------------------------------------------

// Constructor
MovingFood::MovingFood(int x, int y) : Food(0, 0)
{
       setLiftetime(std::numeric_limits<uint>::max());
       setValue(2);
       setGrowth(2);

       std::cout<< "    Added MOVING food at " << x << "/" << y <<"\n";
}

void MovingFood::update() 
{
    // Call the base class update method
    Food::update();
    myPRINT("       Calling MOVING Food:Update");

    // Random Generation taken from ChatBot
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_prob(0.0, 1.0);
    std::uniform_int_distribution<> dis_direction(0, 3);

   
    if (dis_prob(gen) < 0.9)
    {
        myPRINT(" -------------------> Food should MOVE")
        int new_x = _coordinates.x;
        int new_y = _coordinates.y;

        // Generate a random direction: 0 = up, 1 = down, 2 = left, 3 = right
        // I don't check for Grid dimensions here but later....if a food moved outside the grid, it 'll be removed
        int direction = dis_direction(gen);
        switch (direction) 
        {
            case 0: // Up
                new_y = getCoord().y - 1;
                break;
            case 1: // Down
                new_y = getCoord().y + 1 ;
                break;
            case 2: // Left
                new_x = getCoord().x - 1;
                break;
            case 3: // Right
                new_x = getCoord().x + 1;
                break;
        }
        setCoord(SDL_Point{new_x, new_y});
    }
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

// Getter for FoodList (with UniquePointers)
std::vector<std::unique_ptr<Food>> Foods::getFoodList() const 
{
    std::vector<std::unique_ptr<Food>> foodListCopy;

    for (const auto& food : _foodList) {
        foodListCopy.push_back(std::make_unique<Food>(*food));
    }

    return foodListCopy;
}

// void Foods::addFoodItemAtPoint(const int x, const int y)
// {
//        MovingFood myFood{};
//        SDL_Point newPoint{x,y};

//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dis(20000, 20000);


//        myFood.setLiftetime(dis(gen));
//        myFood.setValue(1);
//        myFood.setGrowth(1);
//        myFood.setCoord(newPoint);
       
//        _foodList.emplace_back(myFood);
//        _countOfFood = _foodList.size(); 
//        _timeSinceLastFoodSpawn = 0;

//        std::cout<< "    Added Food at " << x << "/" << y <<"\n";
// } 

void Foods::addNewFood(std::vector<SDL_Point>& occupiedList) 
{
        while (_countOfFood < _maxCountOfFood) 
        {
            int x = _dis_width(_gen);
            int y = _dis_height(_gen);
            
            bool pointIsOccupied{false};
            // Check that the location is not occupied by a snake item before placing
            // food.
            for (auto point : occupiedList)
            {
                if ((point.x == x) && (point.y == y))
                {
                    pointIsOccupied |= true;
                    break;                      // if one point is occupied by food or snake, we can stop checking the list and generate another random value
                }
            }

            if (!pointIsOccupied)
            {
                occupiedList.emplace_back(SDL_Point{x,y});

                if (_countOfFood % 2 == 0) {
                    _foodList.emplace_back(std::make_unique<Food>(x, y));
                } else {
                    _foodList.emplace_back(std::make_unique<MovingFood>(x, y));
                }
                _timeSinceLastFoodSpawn = 0;
                _countOfFood = _foodList.size();
                return;
            }
        }
    }

int Foods::checkPositionForFood(const int x, const int y) 
{
    for (size_t i = 0; i < _foodList.size(); ++i) 
    {
        SDL_Point coord = _foodList[i]->getCoord();
        if (coord.x == x && coord.y == y) {
            std::cout << "Found food at " << x << "/" << y << "\n";
            return i;
        }
    }
    return -1; // If there's no food object at that location, return -1
}

void Foods::removeFood(const int index) 
{
    if (index >= 0 && index < static_cast<int>(_foodList.size()))
    {
        std::cout << "Removing food at (" << _foodList[index]->getCoord().x << ", " << _foodList[index]->getCoord().y << ")" << std::endl;
        _foodList.erase(_foodList.begin() + index);
        --_countOfFood;
    } 
    else 
    {
        std::cout << "Invalid index, cannot remove" << std::endl;
    }
}

void Foods::updateFoodList(std::vector<SDL_Point>& occupiedList, int& score, int& growth) 
{
    //Check if a food was eaten
    // the snake head is always at index 0 of the occupied list
    int indexWithFood = checkPositionForFood(occupiedList[0].x, occupiedList[0].y);

    if (indexWithFood != -1) 
    {
        score = _foodList[indexWithFood]->getValue();       
        growth = _foodList[indexWithFood]->getGrowth();       
        removeFood(indexWithFood);
    }

    // Check, if food needs to be removed
    for (auto it = _foodList.begin(); it != _foodList.end(); ) 
    {
        if ((it->get()->getTimeSinceSpawn().count()>it->get()->getLifetime()) ||
            (it->get()->getCoord().x < 0) ||
            (it->get()->getCoord().x > _grid_width) ||
            (it->get()->getCoord().y < 0) ||
            (it->get()->getCoord().y > _grid_height))
        {
            it = _foodList.erase(it);       // Using ERASE makes it safe to remove an item from the vector during the loop!
            _countOfFood--;
        } else {
            ++it;
        }
    }

    // Finally add new food if there's not enough
    while (_countOfFood < _maxCountOfFood)
    {
        addNewFood(occupiedList);
    }
    updateFoods();

    printFoodList();
}

void Foods::updateFoods()
{
    for (auto &food : _foodList) 
    {
        food->update();
    }
}


std::vector<SDL_Point> Foods::getAllFoodCoordinates() const
{
    std::vector<SDL_Point> myPointList{};
    for (auto &food : _foodList)  
    {
        myPointList.emplace_back(food->getCoord());
    }
    return myPointList;
}

void Foods::printFoodList()  
{
    for (const auto &food : _foodList) 
    {
        SDL_Point coord = food->getCoord();
        std::cout << "Food at (" << coord.x << ", " << coord.y << "), "
                    << "AgeDyn: " << (food->getTimeSinceSpawn().count()) << ", "
                    << "Age: " << (food->getAge()) << ", "
                    << "Lifetime: " << food->getLifetime() << ", "
                    << "Value: " << food->getValue() << std::endl;
    }
}