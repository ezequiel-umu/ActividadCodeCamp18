#ifndef ANT_H
#define ANT_H

#include "../engine/Location.h"
#include "../engine/State.h"

class Ant
{
public:
  Location position;
  int team;

  Ant(State & s);

  bool canWalkTo(FWDirection fd);
  void walkTo(FWDirection fd);

protected:
  State &s;
};

#endif