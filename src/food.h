#ifndef FOOD_H
#define FOOD_H

#include <random>
#include <sys/types.h>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "snake.h"
#include <chrono>
#include <random>

#define myDEBUG(X) std::cout << (#X) << "= " << (X) << std::endl;
#define myPRINT(X) std::cout << (X) << std::endl;
#define myPRINT2(X,Y) std::cout << (X) << "  " << (Y) << std::endl;
#define myFUNC std::cout << "\n-------" << __func__ << "-------" << std::endl;

class Food 
{
  public:
    Food();
    // Food () {};
    ~Food () {};

  //Getter/Setter
    SDL_Point getCoord (){return _coordinates;};
    std::chrono::milliseconds  getAge(){return _age;};
    uint getLifetime (){return _lifetime;};
    uint getValue (){ return _value; };
    std::chrono::milliseconds getTimeSinceSpawn() const;
    
    void setCoord (SDL_Point newCoordinate){ _coordinates = newCoordinate;};
    void setLiftetime (uint newLiftetime){_lifetime =  newLiftetime;};
    void setValue (uint newValue){ _value = newValue;};
    void setGrowth (uint newGrowth){ _growth = newGrowth;};

  // Public Functions

  void update();


  private:
    uint _value;
    uint _growth;
    std::chrono::milliseconds _age;
    uint _lifetime;
    SDL_Point _coordinates;
    std::chrono::time_point<std::chrono::steady_clock> _spawnTime;

    void setAge (std::chrono::milliseconds  newAgeInSec){ _age = newAgeInSec;};
};
 // ------------------------------------------------------------------------------------------------

class Foods
{
  public:
    Foods(){};
    ~Foods(){};

    std::vector<Food> getFoodList() {return _foodList;};
    void updateFoods();
    void addFoodItem(int x, int y);
    // Food* checkPositionForFood(const int x,const int y);
    // std::vector<Food>::iterator checkPositionForFood(const int x, const int y);
    int checkPositionForFood(const int x, const int y);
    void removeFood(const int index);
    void printFoodList(); 
    void updateFoodList();

  private:

    std::vector<Food> _foodList;
    uint _countOfFood{};
    uint _maxCountOfFood{3};
    uint _timeSinceLastFoodSpawn;



};


#endif