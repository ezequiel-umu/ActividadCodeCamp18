#ifndef ACTION_H
#define ACTION_H

#include "../engine/Ant.h"

class Action
{
public:
  Action(Ant &worker);
  virtual bool canDo() = 0;
  virtual bool finished() = 0;
  virtual void next() = 0;

  

protected:
  Ant &worker;
};

#endif