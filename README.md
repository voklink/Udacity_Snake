# CPPND: Edited Capstone Snake Game
This is an edited repo of the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

# New Features
The player can pause the game by hitting the space bar.

New food types were added to the game. Additionally, several foods are placed on the grid at the same time.
This was done by creating a dedicated food class which manages all the different foods.
Each food type can have specific attributes and behaviours as each food type is implemented as a child class.

Foods can also impact the game by e.g. speeding up the snake, e.g. the food type "HyperFood" will speed up the snake
for 2 seconds. The timer for this implemented as its own thread.

# Addressed Rubric Points
Compiling and Testing: 
- added food.cpp, food.h
- compiles and runs without errors based on the provided repo 

Loops, Functions, I/O:
- A variety of control structures are added to the project.
- The project code is clearly organized into functions.
- The project accepts user input and processes the input --> added additional user input to pause the game
- The project uses arrays or vectors and uses constant variables. --> added variety of vectors (such as _foodList)

Object Oriented Programming:
- One or more classes are added to the project with appropriate access specifiers for class members --> Base class food and child classes were added. "Food" was integrated into the "foods" class.
- Class constructors utilize member initialization lists --> yes
- Classes follow an appropriate inheritance hierarchy with virtual and override functions --> update function of child classes "food"

Memory Management:
- The project makes use of references in function declarations. --> functions make use of "pass-by-reference"
- The project uses destructors appropriately. --> yes
- The project uses scope / Resource Acquisition Is Initialization (RAII) - where appropriate. --> yes
- The project uses smart pointers instead of raw pointers. --> yes- 

Concurrency
- The project uses multithreading and a Promise/Future
 


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
