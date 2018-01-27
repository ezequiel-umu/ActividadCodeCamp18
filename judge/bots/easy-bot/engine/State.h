#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"

class Ant;

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int rows, cols,
        turn, turns,
        noPlayers;

    bool newWater;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;
    int64_t seed;

    std::vector<std::vector<Square> > grid;
    std::vector<Location> myAnts, enemyAnts, myHills, enemyHills, food;
    std::vector<Ant> theAnts;

    Timer timer;
    Bug bug;

    /*
        Functions
    */
    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);
    bool canMoveTo(const Location &loc, int direction);

    double distance(const Location &loc1, const Location &loc2);
    double distance2(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &startLoc, int direction);

    void updateVisionInformation();
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);

#endif //STATE_H_
