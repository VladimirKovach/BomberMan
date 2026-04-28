#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

const double EXPLOSION_TIMER = 3.0;
const double EXPLOSION_DURATION = 2.0;

class Bomb {
protected:
    Position p;
    int range;

    bool active;
    bool exploded;

    double placement_time;
    double explosion_time;

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();

    bool is_active();

    bool is_exploded();

    void place(Position _p, int _range, double game_timer);

    void explode(Map& map, double game_timer);

    void update(Map& map, double game_timer);
};

#endif