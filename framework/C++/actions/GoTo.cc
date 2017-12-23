#include "GoTo.h"

GoTo::GoTo(Ant &worker, Location target) : Action(worker), target(target)
{

}

bool GoTo::canDo()
{
    auto nextStep = GoTo::fw.nextStep(worker.position, target);
    return worker.canWalkTo(nextStep);
}

void GoTo::next()
{
  auto nextStep = GoTo::fw.nextStep(worker.position, target);
  worker.walkTo(nextStep);
}