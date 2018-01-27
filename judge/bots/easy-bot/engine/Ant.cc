#include "Ant.h"
#include "../debug.h"

Ant::Ant(State & s) : s{s} {

}


bool Ant::canWalkTo(FWDirection fd)
{
  if (fd == IMPOSSIBLE)
  {
    return false;
  }
  return s.canMoveTo(position, fd);
}

void Ant::walkTo(FWDirection fd)
{
  s.makeMove(position, fd);
  position = Location(position, fd);
  position.wrap(s.cols, s.rows);
}