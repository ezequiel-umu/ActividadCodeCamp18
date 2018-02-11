#include "linearinfluence.h"
#include "../engine/State.h"
#include "bfs.h"

void enemyAntInfluence(const Location &ant)
{
    State &s = State::getSingleton();

    // ¿Está cerca de una colina aliada?
    int nearBonus = 0;
    for (const Location &hill : s.myHills)
    {
        if (s.distance(hill, ant) <= s.viewradius)
        {
            nearBonus = 20;
        }
    }

    BreadFirstExpansion(ant,
                        [&s, &ant, nearBonus](const Location &l, int distance) {
                            Square &sq = s.getGrid(l);
                            int dist = s.distance(l, ant);
                            if (sq.isWater || dist >= s.viewradius)
                            {
                                return OBSTACLE;
                            }
                            else
                            {
                                sq.influence += s.viewradius - dist + nearBonus;
                                return CONTINUE;
                            }
                        });
}

void friendlyAntInfluence(const Location &ant)
{
    State &s = State::getSingleton();

    // ¿Está cerca de un enemigo?
    int nearBonus = std::max(0, s.getGrid(ant).danger);

    BreadFirstExpansion(ant,
                        [&s, &ant, nearBonus](const Location &l, int distance) {
                            Square &sq = s.getGrid(l);
                            int dist = s.distance(l, ant);
                            if (sq.isWater || dist >= s.viewradius)
                            {
                                return OBSTACLE;
                            }
                            else
                            {
                                sq.influence += dist - s.viewradius + nearBonus;
                                return CONTINUE;
                            }
                        });
}

void friendlyHillInfluence(const Location &hill)
{
    // No hacer nada
}

void enemyHillInfluence(const Location &hill)
{
    State &s = State::getSingleton();

    int nearBonus = 50;
    Square & sq = s.getGrid(hill);
    sq.influence += 100;

    BreadFirstExpansion(hill,
                        [&s, &hill, nearBonus](const Location &l, int distance) {
                            Square &sq = s.getGrid(l);
                            int dist = s.distance(l, hill);
                            if (sq.isWater || dist >= s.viewradius)
                            {
                                return OBSTACLE;
                            }
                            else
                            {
                                sq.influence += s.viewradius - dist + nearBonus;
                                return CONTINUE;
                            }
                        });
}

void antInfluence(const Location &ant)
{
    State &s = State::getSingleton();
    auto &sq = s.getGrid(ant);
    if (sq.ant == 0)
    {
        friendlyAntInfluence(ant);
    }
    else if (sq.ant > 0)
    {
        enemyAntInfluence(ant);
    }
}

void hillInfluence(const Location &hill)
{
    State &s = State::getSingleton();
    auto &sq = s.getGrid(hill);
    if (sq.hillPlayer == 0)
    {
        friendlyHillInfluence(hill);
    }
    else if (sq.hillPlayer > 0)
    {
        enemyHillInfluence(hill);
    }
}

void foodInfluence(const Location &food)
{
    State &s = State::getSingleton();

    BreadFirstExpansion(food,
                        [&s, &food](const Location &l, int distance) {
                            Square &sq = s.getGrid(l);
                            int dist = s.distance(l, food);
                            if (sq.isWater || dist >= s.viewradius)
                            {
                                return OBSTACLE;
                            }
                            else
                            {
                                sq.influence += (s.viewradius - dist);
                                return CONTINUE;
                            }
                        });
}