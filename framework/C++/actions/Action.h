#ifndef ACTION_H
#define ACTION_H

#include "../engine/Location.h"

class Action
{
public:
  Action(const Location &worker);
  virtual bool canDo() = 0;
  virtual bool finished() = 0;
  virtual void next() = 0;
  virtual const std::string & actionName() const = 0;

  virtual ~Action() = 0;
protected:
  Location worker;
};

#endif