#include "algorithm.h"
#include <vector>

using std::vector;

struct P {
  int x;
  int y;
};

P to2D(size_t i, size_t width);

struct FWNode {
  size_t distance;
  FWDirection nextStep;
};

class FloydWarshall : public Algorithm
{
private:
  vector<vector<FWNode>> grid;
  int width;
public:
  void init(State &s);
  FWDirection nextStep(Location origin, Location target);
};
