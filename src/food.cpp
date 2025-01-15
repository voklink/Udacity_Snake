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
    std::uniform_int_distribution<> dis(3000, 10000);

    setLiftetime(dis(gen));
    setValue(1);
    setGrowth(1);
    _isHyper = false;
}

std::chrono::milliseconds Food::getTimeSinceSpawn() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _spawnTime);
}

void Food::update(std::vector<SDL_Point>& occupiedList)
{
    (void) occupiedList;
    setAge(getTimeSinceSpawn().count());
}

// --------------------------------------------------------------------------
// Child class MovingFood
// --------------------------------------------------------------------------

// Constructor
MovingFood::MovingFood(int x, int y) : Food(x, y)
{
       setLiftetime(std::numeric_limits<uint>::max());
       setValue(2);
       setGrowth(2);
       _isHyper = false;

       std::cout<< "    Added MOVING food at " << x << "/" << y <<"\n";
}

void MovingFood::update(std::vector<SDL_Point>& occupiedList) 
{
    // Call the base class update method
    Food::update(occupiedList);

    // Random Generation taken from ChatBot
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_prob(0.0, 1.0);
    std::uniform_int_distribution<> dis_direction(0, 3);

   
    if (dis_prob(gen) < 0.03)
    {
        SDL_Point newPoint{_coordinates.x, _coordinates.y};

        // Generate a random direction: 0 = up, 1 = down, 2 = left, 3 = right
        // I don't check for Grid dimensions here but later....if a food moved outside the grid, it 'll be removed
        // Do this until the potential new location is not occpied by food or snake
        while((isPointInList(newPoint, occupiedList)))
        {
            int direction = dis_direction(gen);
            switch (direction) 
            {
                case 0: // Up
                    newPoint.y = getCoord().y - 1;
                    break;
                case 1: // Down
                    newPoint.y = getCoord().y + 1 ;
                    break;
                case 2: // Left
                    newPoint.x = getCoord().x - 1;
                    break;
                case 3: // Right
                    newPoint.x = getCoord().x + 1;
                    break;
            }
        }
        setCoord(newPoint);
        occupiedList.emplace_back(newPoint);
    }
}

bool MovingFood::isPointInList(const SDL_Point& point, std::vector<SDL_Point>& pointList) 
{    
    for (const auto& p : pointList) 
    {
        if (p.x == point.x && p.y == point.y) 
        {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------
// Child class Hyperood
// --------------------------------------------------------------------------

// Constructor
HyperFood::HyperFood(int x, int y) : Food(x, y)
{
    //    setLiftetime(std::numeric_limits<uint>::max());
       setValue(5);
       setGrowth(5);
       _isHyper = true;

       std::cout<< "    Added HYPER food at " << x << "/" << y <<"\n";
}



// --------------------------------------------------------------------------
// Class FOODS
// --------------------------------------------------------------------------

// Constructor with init list
Foods::Foods(int grid_width, int grid_height)
    : _grid_width(grid_width),
      _grid_height(grid_height),
      _gen(_rd()),
      _dis_width(0, grid_width - 1),
      _dis_height(0, grid_height - 1) {}

// Getter for FoodList (with UniquePointers)
// returns a copy!
std::vector<std::unique_ptr<Food>> Foods::getFoodList() const 
{
    std::vector<std::unique_ptr<Food>> foodListCopy;

    for (const auto& food : _foodList) {
        foodListCopy.push_back(std::make_unique<Food>(*food));
    }

    return foodListCopy;
}

void Foods::addNewFood(std::vector<SDL_Point>& occupiedList) 
{
        while (_countOfFood < _maxCountOfFood) 
        {
            int x = _dis_width(_gen);
            int y = _dis_height(_gen);
            
            bool pointIsOccupied{false};
            // Check that the location is not occupied by a snake or other food item before placing
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


                std::uniform_int_distribution<> dis_typeOfFood(0, 2);

                  int typeOfFood = dis_typeOfFood(_gen);
                switch (typeOfFood) 
                {
                    case 0:
                        _foodList.emplace_back(std::make_unique<Food>(x, y));
                        break;
                    case 1: 
                        _foodList.emplace_back(std::make_unique<MovingFood>(x, y));
                        break;
                    case 2: 
                        _foodList.emplace_back(std::make_unique<HyperFood>(x, y));
                        break;
                }
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

void Foods::updateFoodList(std::vector<SDL_Point>& occupiedList, int& score, int& growth, bool& isHyper) 
{
    //Check if a food was eaten
    // the snake head is always at index 0 of the occupied list
    int indexWithFood = checkPositionForFood(occupiedList[0].x, occupiedList[0].y);

    if (indexWithFood != -1) 
    {
        score = _foodList[indexWithFood]->getValue();       
        growth = _foodList[indexWithFood]->getGrowth();  
        isHyper = _foodList[indexWithFood]->getHyper();     
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
    updateFoodItems(occupiedList);

    printFoodList();
}

void Foods::updateFoodItems(std::vector<SDL_Point>& occupiedList)
{
    for (auto &food : _foodList) 
    {
        food->update(occupiedList);
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
                    << "Age: " << (food->getAge()) << ", "
                    << "Lifetime: " << food->getLifetime() << ", "
                    << "Value: " << food->getValue() << std::endl;
    }
}