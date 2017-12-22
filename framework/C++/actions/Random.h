#ifndef RANDOM_ACTION_H
#define RANDOM_ACTION_H

#include "Action.h"

class Random : public Action
{
public:
  /**
   * Construir la acción random
   */
  Random(Ant &worker);
  /**
   * Contruir la acción random, pero solo para andar aleatoriamente `times` veces
   */
  Random(Ant &worker, int times);
  bool canDo() = 0;
  bool finished() = 0;
  void next() = 0;
protected:
  int times;
};

#endif