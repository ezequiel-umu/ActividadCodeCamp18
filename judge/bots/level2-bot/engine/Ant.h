#ifndef ANT_H
#define ANT_H

#include "../engine/Location.h"
#include "../engine/State.h"

class Action;

class Ant
{
public:
  Location position;

  Ant(State & s);
  ~Ant();

  bool canWalkTo(FWDirection fd);
  void walkTo(FWDirection fd);

  Action * action = nullptr;

protected:
  State &s;
};

#endif