#ifndef GOTO_ACTION_H
#define GOTO_ACTION_H

#include "Action.h"
#include "../engine/Location.h"

class GoTo: public Action {
public:

    GoTo() = delete;
    GoTo(Ant&) = delete; 

    GoTo(Ant & ant, FWDirection target);

    bool canDo();
    void next();
    bool finished();
    const std::string & actionName() const;
protected:
    FWDirection target;
};

#endif