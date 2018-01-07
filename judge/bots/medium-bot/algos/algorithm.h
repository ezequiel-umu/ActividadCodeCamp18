#include "../engine/Bug.h"
#include "../engine/State.h"

class Algorithm {
  protected:
    bool isReady = false;
    Bug b;
  public:
    virtual void endTurn() {
      isReady = false;
    }
    virtual void init(State & s) {
      isReady = true; 
    };
};
