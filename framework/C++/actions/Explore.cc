#include <climits>
#include "Explore.h"
#include "../algos/astar.h"
#include "../engine/State.h"
#include "../engine/Location.h"
#include "../debug.h"

const std::string actionName = "EXPLORE";

const int EXPLORE[8][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };      //{N, E, S, W}

Explore::Explore(Ant &worker) : Action(worker)
{
    distance = 8; 
}

Explore::Explore(Ant &worker, int distance) : Action(worker), distance(distance)
{
  
}

bool Explore::canDo()
{
    State & s = State::getSingleton();
    double worstDensity = s.rows * s.cols;
    for (const auto & e : EXPLORE) {
        auto pos = Location(e[0], e[1]) * 3 + worker.position;
        pos.wrap(s.cols, s.rows);
        double density = 0; 
        for (const Location & l: s.myAnts) {
            density += 1/s.distance2(l,pos);
        }
        if (worstDensity > density && !s.getGrid(pos).isWater) {
            worstDensity = density;
            gt = AStar(worker.position, pos);
        }
    } 
    if (gt.size() > 0) {
        return worker.canWalkTo(gt[0].origin);
    }

    return false;
}

void Explore::next()
{
    if (gt.size() > 0)
    {
        worker.walkTo(gt[0].origin);
    }
}

bool Explore::finished() {
    // TODO: this is false
    return false;
}


const std::string & Explore::actionName() const {
    return ::actionName;
}