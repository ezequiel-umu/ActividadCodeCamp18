#ifndef GOTO_ACTION_H
#define GOTO_ACTION_H

#include "Action.h"
#include "../algos/Path.h"
#include "../engine/Location.h"

class GoTo: public Action {
public:

    GoTo() = delete;
    GoTo(Ant&) = delete; 

    GoTo(Ant & ant, FWDirection target);
    GoTo(Ant & ant, const Path & target);

    bool canDo();
    void next();
    bool finished();
    const std::string & actionName() const;
    int getDistance();
protected:
    FWDirection target;
    int distance;
};

#endif