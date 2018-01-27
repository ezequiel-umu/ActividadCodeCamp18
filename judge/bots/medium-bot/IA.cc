#include "IA.h"
#include "actions/GoTo.h"
#include "actions/Random.h"
#include "algos/bfs.h"
#include <climits>

using namespace std;

void IA::init(State & s) {
    for (auto food: s.food) {
        Path nearestAnt = findNearestAnt(s, food);
        if (nearestAnt.size() > 1) {
            Ant & ant = s.theAnts[s.getGrid(nearestAnt[0].point).theAnt];
            getDebugger() << &ant << endl;
            getDebugger() << ant.position << " " << nearestAnt[0].point << endl;
            int distance = INT_MAX;
            getDebugger() << ant.action << " PLS" << std::endl;
            if (ant.action != nullptr && ant.action->actionName() == "GOTO") {                
                distance = ((GoTo*)(ant.action))->getDistance();
            }
            getDebugger() << "NOPLS" << std::endl;
            if (distance > nearestAnt.size()) {
                delete ant.action;
                GoTo * action = new GoTo(ant, nearestAnt);
                if (action->canDo()) {
                    ant.action = action;
                } else {
                    ant.action = new Random(ant);
                }
            }
            getDebugger() << "END IF" << endl;
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
