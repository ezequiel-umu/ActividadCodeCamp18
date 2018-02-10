#ifndef ANT_H
#define ANT_H

#include "../engine/Location.h"

class State;
class Action;

class Ant
{
public:
  Location position;

  Ant(State & s);
  ~Ant();

  bool canWalkTo(FWDirection fd);
  void walkTo(FWDirection fd);

  Action * action = nullptr;

  bool operator==(const Ant & another) const;

  void operator=(Ant & another) = delete;
  void operator=(const Ant & another) const = delete;

  bool alreadyMoved;

protected:
  State &s;
};

namespace std
{
    template<> struct hash<Ant>
    {
        typedef Ant argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            return std::hash<Location>{}(s.position);
        }
    };
}

#endif