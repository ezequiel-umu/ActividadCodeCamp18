#ifndef BFS_H
#define BFS_H

#include "Path.h"
#include "../engine/State.h"

/**
 * El parámetro team indica el equipo. Si es 0,
 * la hormiga es nuestra. Devuelve un Path indicando la hormiga más cercana 
 * a la localización dada. Si no hay ninguna, el Path es vacío.
 */
Path findNearestAnt(const State & s, const Location & l, int team = 0);

/**
 * Localiza la casilla más cercana sin explorar. El límite indica la distancia mínima
 * hasta donde se llega a buscar, pero se podría llegar más lejos.
 * Si el camino es vacío, no se encontró ninguna bajo ese límite.
 */
Path findNearestFog(const State & s, const Location & l, int limit = 15);

#endif