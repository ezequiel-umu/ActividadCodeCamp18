#ifndef SUPPORT_ACTION_H
#define SUPPORT_ACTION_H

#include "Action.h"
#include "GoTo.h"
#include "../algos/Path.h"

class Support : public Action
{
public:
  /**
   * Construir la acción Support
   */
  Support(Ant &worker);
  ~Support();
  bool canDo();
  bool finished();
  void next();
  const std::string & actionName() const;
protected:
  GoTo * internalAction;
  Location frontier;
};

#endif