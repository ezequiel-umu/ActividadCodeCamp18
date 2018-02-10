#include "Bot.h"
#include "../defines.h"
#include "../debug.h"
#include "../algos/danger.h"

using namespace std;

//constructor
Bot::Bot(Scheduler & sch) : sch{sch}
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        updateAlgorithms();
        makeMoves();
        endTurn();
    }
};

void Bot::updateAlgorithms() {
  #ifdef CALCULATE_DANGER
  calculateDanger(state);
  #endif
}

//makes the bots moves for the turn
void Bot::makeMoves()
{
    getDebugger() << "turn " << state.turn << ":" << endl;
    sch.init();
    
    //getDebugger() << state << endl;

    sch.finish();
    getDebugger() << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
