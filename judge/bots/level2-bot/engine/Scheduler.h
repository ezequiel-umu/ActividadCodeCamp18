#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "State.h"
#include <unordered_map>


class Scheduler {
public:
    virtual void init(State & s) = 0;
    virtual void finish(State & s) = 0;
};

#endif