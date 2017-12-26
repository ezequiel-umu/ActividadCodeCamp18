#include "IA.h"
#include "actions/GoTo.h"
#include "actions/Random.h"
#include "algos/bfs.h"

void IA::init(State & s) {
    for (auto food: s.food) {
        Path nearestAnt = findNearestAnt(s, food);
        if (nearestAnt.size() > 1) {
            Ant & ant = *s.getGrid(nearestAnt[0].point).theAnt;
            if (!ant.action) {
                FWDirection dir = nearestAnt[0].origin;
                GoTo * action = new GoTo(ant, dir);
                if (action->canDo()) {
                    ant.action = action;
                } else {
                    ant.action = new Random(ant);
                }
            }
        }
    }

    for (Ant & ant: s.theAnts) {
        ant.action = (ant.action ? ant.action : new Random(ant));
        if (ant.action->canDo()) {
            ant.action->next();
        }
    }
}

void IA::finish(State & s) {

}
