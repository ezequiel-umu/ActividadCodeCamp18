#include "Fight.h"

#include "../debug.h"
#include "../engine/State.h"
#include <climits>

Fight::Fight(Ant &worker) : Action(worker), fightDirection(IMPOSSIBLE)
{

}

bool Fight::canDo()
{ 
  State & s = State::getSingleton();
  int maxDanger = INT_MIN;
  for (auto dir : FDIRECTIONS)
  {
    // Luchar significa andar en dirección de casillas
    // con presencia enemiga, pero siempre aquellas con valores no positivos
    // para evitar suicidios
    Location l(worker.position, dir);
    l.wrap(s.cols, s.rows);
    Square & sq = s.getGrid(l);
    if (maxDanger < sq.danger && sq.danger <= 0 && sq.enemyPresence.size()) {
      maxDanger = sq.danger;
      fightDirection = dir;
    }
  }

  return maxDanger != INT_MIN;
}

void Fight::next()
{
    State & s = State::getSingleton();
    int maxDanger = INT_MIN;
    for (auto dir : FDIRECTIONS)
    {
      // Luchar significa andar en dirección de casillas
      // con presencia enemiga, pero siempre aquellas con valores no positivos
      // para evitar suicidios
      Location l(worker.position, dir);
      l.wrap(s.cols, s.rows);
      Square & sq = s.getGrid(l);
      if (maxDanger < sq.danger && sq.danger < 0 && sq.enemyPresence.size() && sq.isWalkable()) {
        maxDanger = sq.danger;
        fightDirection = dir;
      }
    }

    // Si moverse da el mismo valor que quedarse en el sitio, no moverse:
    if (maxDanger == s.getGrid(worker.position).danger) {
      return;
    }

    if (worker.canWalkTo(fightDirection))
    {
        worker.walkTo(fightDirection);
    }
}

const std::string actionName = "FIGHT";

const std::string & Fight::actionName() const {
  return ::actionName;
}

bool Fight::finished() {
  return true;
}
