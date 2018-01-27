#ifndef SQUARE_H_
#define SQUARE_H_

#include <vector>

class Ant;

/*
    struct for representing a square in the grid.
*/
struct Square
{
  bool isVisible, isWater, isHill, isFood;
  int ant, hillPlayer;
  int theAnt;
  std::vector<int> deadAnts;

  Square()
  {
    isVisible = isWater = isHill = isFood = 0;
    ant = hillPlayer = -1;
    theAnt = -1;
  };

  /**
   * Devuelve verdadero si se puede andar: no hay otra hormiga ni hay agua.
   */
  bool isWalkable()
  {
    return !isWater && ant == -1;
  }

  //resets the information for the square except water information
  void reset()
  {
    isVisible = 0;
    isHill = 0;
    isFood = 0;
    ant = hillPlayer = -1;
    theAnt = -1;
    deadAnts.clear();
  };
};

#endif //SQUARE_H_
