#include "Ant.h"

bool Ant::canWalkTo(FWDirection fd)
{
  if (fd == IMPOSSIBLE)
  {
    return false;
  }
  // Calcular el punto en base a la dirección.
  Location pWalkTo(position, fd);
  // Si se sale por un lado del mapa, obtener su posición dentro.
  pWalkTo.wrap(s);

  Square sq = s.grid[pWalkTo.col][pWalkTo.row];
  return sq.isWalkable();
}

void Ant::walkTo(FWDirection fd)
{
  s.makeMove(position, fd);
  position = Location(position, fd);
}