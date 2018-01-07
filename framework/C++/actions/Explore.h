#ifndef EXPLORE_ACTION_H
#define EXPLORE_ACTION_H

#include "Action.h"
#include "../algos/Path.h"

class Explore : public Action
{
public:
  /**
   * Construir la acción explore
   */
  Explore(Ant &worker);
  /**
   * Construir la acción explore, indicando la distancia aproximada a la que ir a explorar
   */
  Explore(Ant &worker, int distance);
  bool canDo();
  bool finished();
  void next();
  const std::string & actionName() const;
protected:
  int distance;
  Path fog;
};

#endif