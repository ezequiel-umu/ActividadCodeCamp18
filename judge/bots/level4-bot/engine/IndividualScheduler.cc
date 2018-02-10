#include "IndividualScheduler.h"
#include <iostream>
#include "../debug.h"

using namespace std;

void IndividualScheduler::scheduleAnt(Ant & ant) {
    auto * command = this->commandAnt(ant);
    if (command->canDo()) {
        command->next();
        if (statistics.count(command->actionName()) == 0) {
            statistics[command->actionName()] += 1;
        } else {
            statistics[command->actionName()] = 1;
        }
    } else {
        conflicts.insert(&ant);
        if (statistics.count("ACTION_CONFLICT") == 0) {
            statistics["ACTION_CONFLICT"] += 1;
        } else {
            statistics["ACTION_CONFLICT"] = 1;
        }
    }
    delete command;    
}

void IndividualScheduler::init() {
    State & s = State::getSingleton();
    this->statistics.clear();
    this->conflicts.clear();

    for (int ant = 0; ant < (int)s.theAnts.size(); ant++) {
        scheduleAnt(s.theAnts[ant]);
    }
}

bool IndividualScheduler::solveConflicts() {
    std::unordered_set<Ant*> solved;
    for (auto ant: conflicts) {
        auto * command = this->commandAnt(*ant, true);
        if (command->canDo()) {
            command->next();
            if (statistics.count(command->actionName()) == 0) {
                statistics[command->actionName()] += 1;
            } else {
                statistics[command->actionName()] = 1;
            }
            solved.insert(ant);
            statistics["ACTION_CONFLICT"] -= 1;
        }
        delete command;
    }
    conflicts.erase(solved.begin(), solved.end());
    return solved.size() > 0;
}

void IndividualScheduler::finish() {
    // ¿No hacer nada?
}