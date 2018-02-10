#ifndef INDIVIDUAL_SCHEDULER_H
#define INDIVIDUAL_SCHEDULER_H

#include "../actions/Action.h"
#include "Scheduler.h"
#include "Ant.h"
#include <unordered_set>

class IndividualScheduler: public Scheduler {
protected:
    std::unordered_set<Ant *> conflicts;    
    std::unordered_map<std::string, int> statistics;

    virtual Action * commandAnt(Ant & ant, bool isConflict = false) = 0;

public:

    void init();
    void scheduleAnt(Ant & ant);
    bool solveConflicts();
    void finish();
};

#endif