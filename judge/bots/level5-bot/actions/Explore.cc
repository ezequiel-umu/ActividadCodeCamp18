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
        int delta = 3;
        Location pos;
        do {
            pos = Location(e[0], e[1]) * delta + worker.position;
            pos.wrap(s.cols, s.rows);
            delta += 1;
        } while (s.getGrid(pos).isWater && delta < 20);

        double density = 0; 
        for (const Location & l: s.myAnts) {
            density += 1/s.distance2(l,pos);
        }
        for (const Location & l: s.enemyAnts) {
            density -= 5/s.distance2(l,pos);
        }
        for (const Location & l: s.myHills) {
            density -= 3/s.distance2(l,pos);
        }
        for (const Location & l: s.enemyHills) {
            density -= 50/s.distance2(l,pos);
        }
        if (worstDensity > density) {
            getDebugger() << worker.position << " exploring to " << pos << std::endl;
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