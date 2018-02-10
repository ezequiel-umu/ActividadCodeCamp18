#include "../engine/Ant.h"
#include "danger.h"
#include <queue>
#include <unordered_set>
#include "../debug.h"
#include "Antgroup.h"

#define HARDCODED_AGGRO_DISTANCE 17

using namespace std;

void calculateDanger(State & s) {
    auto & enemies = s.enemyAnts;
    for (const Location & ant: enemies) {
        queue<Location> locs;
        unordered_set<Location> visited; 

        locs.push(ant);
        visited.insert(ant);
        while (locs.size()) {
            const Location & l = locs.front(); locs.pop();
            Square & sq = s.getGrid(l);
            sq.danger += 1;
            sq.enemyPresence.insert(ant);

            // if (sq.enemyPresence.size() > 4) {
            //   getDebugger() << ant << " what" << endl;
            //   for (const Location & l: sq.enemyPresence) {
            //     getDebugger() << l << endl;                
            //   }
            // }

            for (auto dir: FDIRECTIONS) {
                Location nl(l, dir);
                nl.wrap(s.cols, s.rows);
                if (visited.count(nl) == 0 && s.distance(ant, nl) <= HARDCODED_AGGRO_DISTANCE) {
                    locs.push(nl);
                    visited.insert(nl);
                }
            }
        }
    }

    auto & myAnts = s.myAnts;
    for (const Location & ant: myAnts) {
        queue<Location> locs;
        unordered_set<Location> visited; 

        locs.push(ant);
        visited.insert(ant);
        while (locs.size()) {
            const Location & l = locs.front(); locs.pop();
            Square & sq = s.getGrid(l);
            if (sq.isWater) {
              continue;
            }
            sq.danger -= 1;
            sq.ownPresence.insert(ant);

            // if (sq.ownPresence.size() > 4) {
            //   getDebugger() << myAnts.size() << endl;              
            //   getDebugger() << l << " what" << endl;
            //   for (const Location & l2: sq.ownPresence) {
            //     getDebugger() << l2 << " " << s.distance(l, l2) << endl;
            //   }
            // }

            for (auto dir: FDIRECTIONS) {
                Location nl(l, dir);
                nl.wrap(s.cols, s.rows);
                if (visited.count(nl) == 0 && s.distance(ant, nl) <= HARDCODED_AGGRO_DISTANCE) {
                    locs.push(nl);
                    visited.insert(nl);
                }
            }
        }
    }
}