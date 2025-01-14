#ifndef FOOD_H
#define FOOD_H

#include <cstdint>
#include <random>
#include <sys/types.h>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "snake.h"
#include <chrono>
#include <random>
#include <memory>

#define myDEBUG(X) std::cout << (#X) << "= " << (X) << std::endl;
#define myPRINT(X) std::cout << (X) << std::endl;
#define myPRINT2(X,Y) std::cout << (X) << "  " << (Y) << std::endl;
#define myFUNC std::cout << "\n-------" << __func__ << "-------" << std::endl;


// --------------------------------------------------------------------------
// class FOOD 
// --------------------------------------------------------------------------
class Food 
{
  public:
    Food(int x, int y);
    virtual ~Food () {};

  //Getter/Setter
    SDL_Point getCoord () const {return _coordinates;} ;
    int32_t getAge(){return _age;};
    uint getLifetime (){return _lifetime;};
    uint getValue (){ return _value; };
    uint getGrowth (){ return _growth; };
    uint getHyper (){ return _isHyper; };
    std::chrono::milliseconds getTimeSinceSpawn() const;
    
    void setCoord (SDL_Point newCoordinate){ _coordinates = newCoordinate;};
    void setLiftetime (uint newLiftetime){_lifetime =  newLiftetime;};
    void setValue (uint newValue){ _value = newValue;};
    void setGrowth (uint newGrowth){ _growth = newGrowth;};

  // Public Functions

  virtual void update(std::vector<SDL_Point>& occupiedList);


  protected:
    SDL_Point _coordinates;
    uint _value;
    uint _growth;
    int32_t _age;
    uint _lifetime;
    bool _isHyper;
    std::chrono::time_point<std::chrono::steady_clock> _spawnTime;

    void setAge (int32_t  newAgeInSec){ _age = newAgeInSec;};
};
 // ------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------
// child class MovingFOOD 
// --------------------------------------------------------------------------
  class MovingFood : public Food {
  public:
      MovingFood(int x, int y);

      void update(std::vector<SDL_Point>& occupiedList) override;
      bool isPointInList(const SDL_Point& point, std::vector<SDL_Point>& pointList); 


  };

  // --------------------------------------------------------------------------
// child class HyperFOOD 
// --------------------------------------------------------------------------
  class HyperFood : public Food {
  public:
      HyperFood(int x, int y);


  };

// --------------------------------------------------------------------------
// Class FOODS  
// --------------------------------------------------------------------------
class Foods
{
  public:
    Foods(int grid_width, int grid_height);
    ~Foods(){};

    std::vector<std::unique_ptr<Food>> getFoodList() const; 
    void updateFoodItems(std::vector<SDL_Point>& occupiedList);
    int checkPositionForFood(const int x, const int y);
    void removeFood(const int index);
    void printFoodList(); 
    void updateFoodList(std::vector<SDL_Point>& occupiedList, int& score, int& growth, bool& isHyper);
    std::vector<SDL_Point> getAllFoodCoordinates() const;
    void addNewFood(std::vector<SDL_Point>& occupiedList) ;

  private:

    std::vector<std::unique_ptr<Food>> _foodList; 
    uint _countOfFood{};
    uint _maxCountOfFood{5};
    int _grid_width;
    int _grid_height;

    // Members for Random generation
    // taken from ChatBot
    std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_int_distribution<> _dis_width;
    std::uniform_int_distribution<> _dis_height;

};


#endif