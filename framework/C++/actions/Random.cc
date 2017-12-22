#include "Random.h"
#include "../engine/State.h"
#include "../engine/Location.h"

Random::Random(Ant &worker) : Action(worker)
{
}

Random::Random(Ant &worker, int times) : Action(worker), times(times)
{
}

bool Random::canDo()
{
  for (auto dir : FDIRECTIONS)
  {
    if (worker.canWalkTo(dir))
    {
      return true;
    }
  }
  return false;
}

void Random::next()
{
  std::vector<FWDirection> v;

  for (auto dir : FDIRECTIONS)
  {
    if (worker.canWalkTo(dir))
    {
      v.push_back(dir);
    }
  } 

  if (v.size())
  {
    // TODO: No usar este random?
    size_t i = rand() % v.size();
    worker.walkTo(v[i]);
  }
}