#ifndef FLEE_ACTION_H
#define FLEE_ACTION_H

#include "Action.h"
#include "../algos/Path.h"
#include "../engine/Location.h"

class Flee: public Action {
public:

    Flee(Ant & ant);

    bool canDo();
    void next();
    bool finished();
    const std::string & actionName() const;
};

#endif