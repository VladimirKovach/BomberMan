#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

const int EXPLOSION_DURATION = 30;  // frame di persistenza dell'esplosione

const int DEFAULT_RANGE = 1;
const double TIMER = 3.0;
//const double EXPLOSION_DURATION = 2.0;

class Bomb {
protected:
    Position p;
    int range;
    double place_time;
    bool exploded;

public:
    Bomb(Position _p = {-1, -1}, int _range = DEFAULT_RANGE);

    Position get_position();

    bool is_exploded();

    bool is_timer_finished(double game_timer);

    void place(Position _p, double game_timer, int _range = DEFAULT_RANGE);

    void explode(Map& map);

    void reset();
};

#endif