#ifndef FOOD_H
#define FOOD_H

#include <random>
#include <sys/types.h>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "snake.h"

#define myDEBUG(X) std::cout << (#X) << "= " << (X) << std::endl;
#define myPRINT(X) std::cout << (X) << std::endl;
#define myPRINT2(X,Y) std::cout << (X) << "  " << (Y) << std::endl;
#define myFUNC std::cout << "\n-------" << __func__ << "-------" << std::endl;

class Food 
{
  public:
    Food () {};
    ~Food () {};

  //Getter/Setter
    SDL_Point getCoord (){return _coordinates;};
    uint getAge (){return _age;};
    uint getLiftetime (){return _lifetime;};
    uint getValue (){ return _value; };
    
    void setCoord (SDL_Point newCoordinate){ _coordinates = newCoordinate;};
    void setLiftetime (uint newLiftetime){_lifetime =  newLiftetime;};
    void setValue (uint newValue){ _value = newValue;};
    void setGrowth (uint newGrowth){ _growth = newGrowth;};

  // Public Functions

  void update();


  private:
    uint _value;
    uint _growth;
    uint _age;
    uint _lifetime;
    SDL_Point _coordinates;

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
    std::vector<Food>::iterator checkPositionForFood(int x, int y);
    void removeFood(std::vector<Food>::iterator it);
    void printFoodList(); 

  private:

    std::vector<Food> _foodList;
    uint _countOfFood{};
    uint _maxCountOfFood{3};
    uint _timeSinceLastFoodSpawn;



};


#endif