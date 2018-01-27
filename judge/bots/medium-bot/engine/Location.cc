#include "Location.h"

std::ostream & operator<<(std::ostream & o, const Location & l) {
  return o << l.row << ", " << l.col;
}