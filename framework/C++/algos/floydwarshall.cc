#include "floydwarshall.h"

P to2D(size_t i, size_t width) {
  return {
    x: i % width,
    y: i / width,
  };
}

void FloydWarshall::init(State &s)
{
  size_t W = s.grid.size();
  
  // Cambiar el tamaño si el estado tiene un tamaño distinto.
  size_t V = W * s.grid[0].size();


  if (this->grid.size() != V) {
    this->grid.resize(V);
  }

  for (int i = 0; i < V; i++) {
    for (int j = 0; j < V; j++) {
      for (int k = 0; k < V; k++) {
        P punto1 = to2D(i, W);
        P punto2 = to2D(k, W);
        P pivote = to2D(j, W);
      }
    }
  }
}
