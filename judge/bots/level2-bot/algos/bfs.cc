#include "bfs.h"

#include <queue>
#include <unordered_set>
#include "../engine/Location.h"
#include "../debug.h"

using namespace std;

Path findNearestAnt(const State & s, const Location & l, int team) {
    queue<Step> nodes;
    unordered_set<Step> visited; 
    Path path;

    nodes.push(Step({l, IMPOSSIBLE}));
    visited.insert(Step({l, IMPOSSIBLE}));

    while(nodes.size()) {
        auto n = nodes.front(); nodes.pop();
        for (auto dir: FDIRECTIONS) {
            Location l(n.point, dir);
            l.wrap(s.cols, s.rows);

            // Evitar agua
            if (!s.getGrid(l).isWater) {
                Step st({l, OPPOSITE[dir]});
                if (!visited.count(st)) {
                    visited.insert(st);
                    nodes.push(st);

                    // Hormiga encontrada
                    if (s.getGrid(l).ant == team) {
                        Step & actSt = st;
                        while (actSt.origin != IMPOSSIBLE) {
                            Location origin(st.point, st.origin);
                            origin.wrap(s.cols, s.rows);
                            path.push_back(st);
                            auto it = visited.find(Step({origin, IMPOSSIBLE}));
                            if (it == visited.end()) {
                                getDebugger() << "crash " << origin << endl;
                            }
                            actSt = *it;
                        }
                        nodes.empty();
                    }
                }
            }
        }
    }

    

    return path;
}