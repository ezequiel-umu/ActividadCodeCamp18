#include "Ant.h"
#include "../debug.h"
#include "../actions/Action.h"
#include "../engine/State.h"


Ant::Ant(State & s) : s{s} {
  action = nullptr;
  alreadyMoved = false;
}

Ant::~Ant() {
  delete this->action;
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
  alreadyMoved = true;
}

bool Ant::operator==(const Ant & another) const {
  return this->position == another.position;
}
