#ifndef EXPLORE_ACTION_H
#define EXPLORE_ACTION_H

#include "Action.h"
#include "../algos/Path.h"

class Explore : public Action
{
public:
  /**
   * Construir la acción explore
   */
  Explore(Ant &worker);
  bool canDo();
  bool finished();
  void next();
  const std::string & actionName() const;
protected:
  FWDirection bestdir;
};

#endif