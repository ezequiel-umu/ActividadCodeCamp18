#include "Flee.h"

#include "../debug.h"
#include "../engine/State.h"

Flee::Flee(Ant &worker) : Action(worker)
{

}

bool Flee::canDo()
{ 
  State & s = State::getSingleton();
  for (auto dir : FDIRECTIONS)
  {
    Location l(worker.position, dir);
    l.wrap(s.cols, s.rows);
    if (worker.canWalkTo(dir) && s.getGrid(l).danger == 0)
    {
      return true;
    }
  }
  return false;
}

void Flee::next()
{
  State & s = State::getSingleton();
  for (auto dir : FDIRECTIONS)
  {
    Location l(worker.position, dir);
    l.wrap(s.cols, s.rows);
    if (worker.canWalkTo(dir) && s.getGrid(l).danger == 0)
    {
      worker.walkTo(dir);
      return;
    }
  }
}

const std::string actionName = "FLEE";

const std::string & Flee::actionName() const {
  return ::actionName;
}

bool Flee::finished() {
  return true;
}
