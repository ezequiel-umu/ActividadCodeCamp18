#include "GoTo.h"

GoTo::GoTo(Ant &worker, FWDirection target) : Action(worker), target(target)
{

}

bool GoTo::canDo()
{ 
  return worker.canWalkTo(target);
}

void GoTo::next()
{
  worker.walkTo(target);
}

const std::string actionName = "GOTO";

const std::string & GoTo::actionName() const {
  return ::actionName;
}

bool GoTo::finished() {
  return true;
}
