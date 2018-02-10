#ifndef ASTAR_H
#define ASTAR_H

#include "Path.h"

/**
 * Encuentra el camino más corto entre dos localizaciones usando la distancia
 * del taxi como heurística.
 */
Path AStar(const Location & to, const Location & from);

#endif