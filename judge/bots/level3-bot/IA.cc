#include "IA.h"
#include "actions/GoTo.h"
#include "actions/Random.h"
#include "actions/Explore.h"
#include "algos/bfs.h"
#include <climits>

using namespace std;

void IA::init(State & s) {
    getDebugger() << "Food: " << s.food.size() << endl;
    int foodLooker = 0;
    // Darle órdenes a las hormigas para buscar comida
    for (auto food: s.food) {
        Path nearestAnt = findNearestAnt(s, food);
        if (nearestAnt.size() > 1) {
            Ant & ant = s.theAnts[s.getGrid(nearestAnt[0].point).theAnt];
            int distance = INT_MAX;
            if (ant.action != nullptr && ant.action->actionName() == "GOTO") {                
                distance = ((GoTo*)(ant.action))->getDistance();
            }
            if (distance > nearestAnt.size()) {
                delete ant.action;
                GoTo * action = new GoTo(ant, nearestAnt);
                if (action->canDo()) {
                    ant.action = action;
                    foodLooker++;
                } else {
                    delete action;
                }
            }
        }
    }

    getDebugger() << "Food seeker: " << foodLooker << endl;

    // Al resto que explore
    for (Ant & ant: s.theAnts) {
        ant.action = (ant.action ? ant.action : new Explore(ant));
        if (ant.action->canDo()) {
            ant.action->next();
        } else {
            delete ant.action;
            ant.action = new Random(ant);
            if (ant.action->canDo()) {
                ant.action->next();
            }
        }
    }
}

void IA::finish(State & s) {

}
