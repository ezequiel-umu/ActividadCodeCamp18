#include "IA.h"
#include "actions/GoTo.h"
#include "actions/Random.h"
#include "actions/Explore.h"
#include "algos/bfs.h"
#include "algos/Antgroup.h"
#include <climits>
#include "algos/minimax.h"

using namespace std;

void IA::init()
{
    unordered_map<Location, Action *> actions;

    State &s = State::getSingleton();
    getDebugger() << "Food: " << s.food.size() << endl;
    int foodLooker = 0;
    // Darle órdenes a las hormigas para buscar comida
    for (auto food : s.food)
    {
        Path nearestAnt = findNearestAnt(food);
        if (nearestAnt.size() > 1)
        {
            Location &ant = nearestAnt[0].point;
            int distance = INT_MAX;
            if (actions[ant] != nullptr && actions[ant]->actionName() == "GOTO")
            {
                distance = ((GoTo *)(actions[ant]))->getDistance();
            }
            if (distance > nearestAnt.size())
            {
                if (actions[ant] == nullptr)
                {
                    foodLooker++;
                }
                delete actions[ant];
                GoTo *action = new GoTo(ant, nearestAnt);
                actions[ant] = action;
            }
        }
    }

    // Darle órdenes a las hormigas para atacar hormigueros
    for (auto hill : s.enemyHills)
    {
        Path nearestAnt = findNearestAnt(hill);
        if (nearestAnt.size() > 1)
        {
            Location &ant = nearestAnt[0].point;
            int distance = INT_MAX;
            if (actions[ant] != nullptr && actions[ant]->actionName() == "GOTO")
            {
                distance = ((GoTo *)(actions[ant]))->getDistance();
            }
            if (distance > nearestAnt.size())
            {
                delete actions[ant];
                GoTo *action = new GoTo(ant, nearestAnt);
                actions[ant] = action;
            }
        }
    }

    getDebugger() << "Food seeker: " << foodLooker << endl;

    // Al resto que luche, huya o explore
    for (Location & ant: s.myAnts) {
        getDebugger() << "General purpose ant: " << ant << endl;
        if (actions[ant] == nullptr) {
            Square & sq = s.getGrid(ant);
            if (sq.enemyPresence.size()) {
                AntGroup ag = AntGroup::getGroupBattleAt(ant, 7);
                getDebugger() << "Group Battle " << ag.size() << endl;
                auto own = ag.getOwnAnts();
                auto enemy = ag.getEnemyAnts();
                if (own.size() != 0 && enemy.size() != 0) {
                    auto d = minimax(own, enemy);
                    getDebugger() << s.timer.getTime() << "ms" << endl;
                    getDebugger() << "Decision made " << d.size() << endl;

                    for (const Decision & dec: d) {
                        getDebugger() << "The Ant " << dec.ant << " to " << dec.to << endl;

                        const Location & ant = dec.ant;

                        getDebugger() << "Location post" << endl;
                        
                        if (actions[ant] != nullptr) {
                            getDebugger() << "Pre GoTo" << endl;                          
                            actions[ant] = new GoTo(ant, dec.to);
                            getDebugger() << "Post GoTo" << endl;                          
                        } else {
                            getDebugger() << "Busy ant" << endl;
                        }
                    }
                }
            } else {
                // Explorar si es posbile
                actions[ant] = new Explore(ant);
                // if (actions[ant]->canDo()) {
                //     actions[ant]->next();
                // } else {
                //     delete actions[ant];
                //     actions[ant] = new Random(ant);
                //     if (actions[ant]->canDo()) {
                //         actions[ant]->next();
                //     }
                // }
            }
        }
    }

    int moved = 1;
    while (moved)
    {
        getDebugger() << "Try" << endl;        
        moved = 0;
        for (auto it = actions.begin(); it != actions.end(); it++)
        {
            if (it->second && it->second->canDo())
            {
                getDebugger() << "next" << endl;
                moved++;
                it->second->next();
                delete it->second;
                it->second = nullptr;
            }
        }
    }

    getDebugger() << "Deleting" << endl;
    for (auto &p : actions)
    {
        if (p.second)
        {
            delete p.second;
        }
    }
}

void IA::finish()
{
}
