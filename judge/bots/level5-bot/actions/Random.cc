#include <climits>
#include "Random.h"
#include "../engine/State.h"
#include "../engine/Location.h"
#include "../debug.h"

const std::string actionName = "RANDOM";

Random::Random(Ant &worker) : Action(worker)
{
  times = INT_MAX;
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
  
  times -= 1;
}

bool Random::finished() {
  return times <= 0;
}


const std::string & Random::actionName() const {
  return ::actionName;
}