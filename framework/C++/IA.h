#ifndef IA_H
#define IA_H

#include "engine/Scheduler.h"
#include "actions/Action.h"

class IA : public Scheduler {
public:
    IA();

    Action * commandAnt(Ant & ant, bool isConflict = false);
};

#endif