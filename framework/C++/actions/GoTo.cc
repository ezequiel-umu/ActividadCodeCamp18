#include "GoTo.h"

#include "../debug.h"

GoTo::GoTo(const Location &worker, FWDirection target) : Action(worker), target(target), distance(1)
{

}

GoTo::GoTo(const Location &worker, const Path & target) : Action(worker), target(target[0].origin), distance(target.size())
{

}

bool GoTo::canDo()
{ 
  if (target == IMPOSSIBLE) {
    return true;
  }
  return worker.canWalkTo(target);
}

void GoTo::next()
{
  if (target != IMPOSSIBLE) {
    worker.walkTo(target);
  }
}

const std::string actionName = "GOTO";

const std::string & GoTo::actionName() const {
  return ::actionName;
}

bool GoTo::finished() {
  return true;
}

int GoTo::getDistance() {
  return this->distance;
}