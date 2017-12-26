#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../actions/Action.h"
#include "Ant.h"
#include <unordered_set>
#include <unordered_map>

class Scheduler {
protected:
    std::unordered_map<std::string, int> statistics;
    std::unordered_set<Ant *> conflicts;
    virtual Action * commandAnt(Ant & ant, bool isConflict = false) = 0;

public:
    void init();
    void scheduleAnt(Ant & ant);
    bool solveConflicts();
    void finish();
};

#endif