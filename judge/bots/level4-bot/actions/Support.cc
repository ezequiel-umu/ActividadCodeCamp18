#include "Support.h"
#include "../algos/bfs.h"
#include "../engine/State.h"
#include "../engine/Location.h"
#include "../debug.h"
#include "../algos/astar.h"
#include "../algos/Antgroup.h"
#include <climits>
#include <unordered_map>

const std::string actionName = "SUPPORT";

Support::Support(Ant &worker) : Action(worker), internalAction(nullptr)
{

}

Support::~Support() {
  delete internalAction;
}

// Caché de batallas
std::unordered_map<Location, AntGroup> battleGroups;
int lastTurn = 0;

bool Support::canDo()
{
    State & state = State::getSingleton();
    // Limpiar la caché de batallas entre turno y turno
    if (lastTurn != state.turn) {
      battleGroups = std::unordered_map<Location, AntGroup>();
      lastTurn = state.turn;
    }
    int str = INT_MAX;

    if (state.enemyAnts.size() == 0) {
        return false;
    }


    for (const Location & l: state.enemyAnts) {
        if (!battleGroups.count(l)) {
          battleGroups[l] = AntGroup::getGroupBattleAt(l, 50);
        }
        auto ag = battleGroups[l];
        if (str > ag.strength) {
            str = ag.strength;
            frontier = l;
        }
    }

    // No ir si ya hay suficiente fuerza
    if (str > 0) {
      return false;
    }

    getDebugger() << str <<  " support at " << frontier << std::endl;

    Path astar = AStar(worker.position, frontier);

    if (astar.size() == 0) {
      return false;
    }

    internalAction = new GoTo(worker, astar);

    return internalAction->canDo();
}

void Support::next()
{
    battleGroups[frontier].strength++;
    State & state = State::getSingleton();
    if (internalAction != nullptr)
        internalAction->next();
}

bool Support::finished() {
    // TODO: this is false
    return false;
}


const std::string & Support::actionName() const {
    return ::actionName;
}