#include <climits>
#include "Explore.h"
#include "../algos/bfs.h"
#include "../engine/State.h"
#include "../engine/Location.h"
#include "../debug.h"

const std::string actionName = "EXPLORE";

Explore::Explore(Ant &worker) : Action(worker)
{
    distance = 50; 
}

Explore::Explore(Ant &worker, int distance) : Action(worker), distance(distance)
{
  
}

bool Explore::canDo()
{
    State & state = State::getSingleton();
    fog = findNearestFog(state, worker.position, this->distance);

    if (fog.size() > 0) {
        auto last = fog.rbegin();
        auto reversed = OPPOSITE[last->origin];
        return worker.canWalkTo(reversed);
    }

    return false;
}

void Explore::next()
{
    State & state = State::getSingleton();
    
    if (fog.size() > 0)
    {
        auto last = fog.rbegin();
        auto reversed = OPPOSITE[last->origin];
        worker.walkTo(reversed);
    }
}

bool Explore::finished() {
    // TODO: this is false
    return false;
}


const std::string & Explore::actionName() const {
    return ::actionName;
}