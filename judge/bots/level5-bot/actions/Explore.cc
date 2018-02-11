#include <climits>
#include "Explore.h"
#include "../algos/astar.h"
#include "../algos/bfs.h"
#include "../engine/State.h"
#include "../engine/Location.h"
#include "../debug.h"

#define WORST_INFLUENCE -999999
#define INFLUENCE_DISTANCE 30

const std::string actionName = "EXPLORE";

Explore::Explore(const Location &worker) : Action(worker)
{
  bestdir = IMPOSSIBLE;
}

bool Explore::canDo()
{
    if (bestdir != IMPOSSIBLE) {
      return worker.canWalkTo(bestdir);
    }

    State &s = State::getSingleton();
    double bestInfluence = WORST_INFLUENCE;
    Location bestLoc;

    BreadFirstExpansion(worker,
        [&s, &bestInfluence, &bestLoc, this](const Location &l, int distance) {
            Square &sq = s.getGrid(l);
            if (distance >= INFLUENCE_DISTANCE || sq.isWater)
            {
                return OBSTACLE;
            }
            else
            {
                if (bestInfluence < sq.influence && !sq.isWater && l != worker) {
                    bestInfluence = sq.influence;
                    bestLoc = l;
                }
                return CONTINUE;
            }
        });
    
    if (bestInfluence > WORST_INFLUENCE + 10) {
        getDebugger() << worker << " wants to go to " << bestLoc << std::endl;
        Path p = AStar(worker, bestLoc);
        bestdir = p[0].origin;
        return worker.canWalkTo(bestdir);
    }
    return false;
}

void Explore::next()
{
    if (bestdir != IMPOSSIBLE)
    {
        worker.walkTo(bestdir);
    }
}

bool Explore::finished()
{
    // TODO: this is false
    return false;
}

const std::string &Explore::actionName() const
{
    return ::actionName;
}