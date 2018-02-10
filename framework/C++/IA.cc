#include "IA.h"
#include "actions/GoTo.h"
#include "actions/Random.h"
#include "actions/Explore.h"
#include "actions/Fight.h"
#include "actions/Flee.h"
#include "actions/Support.h"
#include "algos/bfs.h"
#include "algos/Antgroup.h"
#include <climits>
#include "algos/minimax.h"

using namespace std;

void IA::init() {
    State & s = State::getSingleton();
    getDebugger() << "Food: " << s.food.size() << endl;
    int foodLooker = 0;
    // Darle órdenes a las hormigas para buscar comida
    for (auto food: s.food) {
        Path nearestAnt = findNearestAnt(food);
        if (nearestAnt.size() > 1) {
            Ant & ant = s.theAnts[s.getGrid(nearestAnt[0].point).theAnt];
            int distance = INT_MAX;
            if (ant.action != nullptr && ant.action->actionName() == "GOTO") {                
                distance = ((GoTo*)(ant.action))->getDistance();
            }
            if (distance > nearestAnt.size()) {
                if (ant.action == nullptr) {
                    foodLooker++;
                }
                delete ant.action;
                GoTo * action = new GoTo(ant, nearestAnt);
                if (action->canDo()) {
                    ant.action = action;
                } else {
                    delete action;
                }
            }
        }
    }

    // Darle órdenes a las hormigas para atacar hormigueros
    for (auto hill: s.enemyHills) {
        Path nearestAnt = findNearestAnt(hill);
        if (nearestAnt.size() > 1) {
            Ant & ant = s.theAnts[s.getGrid(nearestAnt[0].point).theAnt];
            int distance = INT_MAX;
            if (ant.action != nullptr && ant.action->actionName() == "GOTO") {                
                distance = ((GoTo*)(ant.action))->getDistance();
            }
            if (distance > nearestAnt.size()) {
                if (ant.action == nullptr) {
                    foodLooker++;
                }
                delete ant.action;
                GoTo * action = new GoTo(ant, nearestAnt);
                if (action->canDo()) {
                    ant.action = action;
                } else {
                    delete action;
                }
            }
        }
    }

    getDebugger() << "Food seeker: " << foodLooker << endl;

    // Al resto que luche, huya o explore
    for (Ant & ant: s.theAnts) {
        if (ant.action == nullptr) {
            getDebugger() << "General purpose ant: " << ant.position << endl;   
            // En peligro: luchar o huir
            Square & sq = s.getGrid(ant.position);
            if (sq.enemyPresence.size()) {
                AntGroup ag = AntGroup::getGroupBattleAt(ant.position, 7);
                getDebugger() << "Group Battle " << ag.size() << endl;
                auto own = ag.getOwnAnts();
                auto enemy = ag.getEnemyAnts();
                auto d = minimax(own, enemy);
                getDebugger() << "Decision made " << d.size() << endl;
                            
                for (const Decision & dec: d) {
                    getDebugger() << "Ant " << dec.ant << " to " << dec.to << endl;
                    
                    Ant & a = s.getAntAt(dec.ant);
                    if (!a.action) {
                        a.action = new GoTo(a, dec.to);
                        a.action->next();
                    } else {
                        getDebugger() << "Busy ant" << endl;                        
                    }
                }
            } else {
                // Explorar si es posbile
                ant.action = new Explore(ant);                
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
        } else {
            // Buscadores de comida
            if (ant.action->canDo()) {
                ant.action->next();
            }
        }
    }
}

void IA::finish() {

}
