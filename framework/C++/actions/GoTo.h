#ifndef GOTO_ACTION_H
#define GOTO_ACTION_H

#include "Action.h"
#include "../engine/Location.h"
#include "../algos/floydwarshall.h"

class GoTo: public Action {
public:
    static FloydWarshall fw;

    GoTo() = delete;
    GoTo(Ant&) = delete; 

    GoTo(Ant & ant, Location target);

    bool canDo();
    void next();
protected:
    Location target;
};

#endif