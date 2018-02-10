#include "bfs.h"

#include <queue>
#include <unordered_set>
#include <math.h>
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

            // Evitar agua y peligro
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
                                getDebugger() << "crash Ant" << origin << endl;
                            }
                            actSt = *it;
                        }
                        return path;
                    }
                }
            }
        }
    }

    return path;
}

Path findNearestFog(const State & s, const Location & l, int limit) {
    queue<Step> nodes;
    unordered_set<Step> visited; 
    Path path;

    nodes.push(Step({l, IMPOSSIBLE}));
    visited.insert(Step({l, IMPOSSIBLE}));

    double realLimit = 2*limit*limit + 2*limit + 1;

    while(nodes.size() && realLimit > 0) {
        auto n = nodes.front(); nodes.pop();
        for (auto dir: FDIRECTIONS) {
            Location l(n.point, dir);
            l.wrap(s.cols, s.rows);

            // Evitar agua y suicidios
            if (!s.getGrid(l).isWater) {
                realLimit--;
                Step st({l, OPPOSITE[dir]});
                if (!visited.count(st)) {
                    visited.insert(st);
                    nodes.push(st);

                    // Niebla encontrada
                    if (!s.getGrid(l).isVisible) {
                        Step & actSt = st;
                        while (actSt.origin != IMPOSSIBLE) {
                            Location origin(st.point, st.origin);
                            origin.wrap(s.cols, s.rows);
                            path.push_back(st);
                            auto it = visited.find(Step({origin, IMPOSSIBLE}));
                            if (it == visited.end()) {
                                getDebugger() << "crash Fog" << origin << endl;
                            }
                            actSt = *it;
                        }
                        return path;
                    }
                }
            }
        }
    }

    return path;
}