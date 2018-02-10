#include "GoTo.h"

GoTo::GoTo(Ant &worker, Location target) : Action(worker), target(target)
{

}

bool GoTo::canDo()
{ 
    // auto nextStep = FloydWarshall::getSingleton().nextStep(worker.position, target);
    // return worker.canWalkTo(nextStep);
    return false;
}

void GoTo::next()
{
  // auto nextStep = FloydWarshall::getSingleton().nextStep(worker.position, target);
  // worker.walkTo(nextStep);
}