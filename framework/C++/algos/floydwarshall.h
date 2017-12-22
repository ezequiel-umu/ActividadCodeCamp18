#include "algorithm.h"
#include <vector>

using std::vector;

enum FWDirection {
  N, 
  E, 
  S, 
  W,
  IMPOSSIBLE,
};

struct P {
  int x;
  int y;
};

P to2D(size_t i, size_t width);

struct FWNode {
  size_t distance;
  vector<vector<FWDirection>> nextStep;
};

class FloydWarshall : public Algorithm
{
private:
  vector<FWNode> grid;
public:
  void init(State &s);
};
