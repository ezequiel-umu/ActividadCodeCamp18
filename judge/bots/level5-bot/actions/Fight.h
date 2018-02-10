#ifndef FIGHT_ACTION_H
#define FIGHT_ACTION_H

#include "Action.h"
#include "../algos/Path.h"
#include "../engine/Location.h"

class Fight: public Action {
public:

    Fight(Ant & ant);

    bool canDo();
    void next();
    bool finished();
    const std::string & actionName() const;
private:
    FWDirection fightDirection;
};

#endif