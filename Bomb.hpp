#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

const int EXPLOSION_DURATION = 30;  // frame di persistenza dell'esplosione

const int DEFAULT_RANGE = 1;
const double DEFAULT_TIMER = 3.0;

class Bomb {
protected:
    Position p;
    int range;
    double timer;
    double place_time;
    bool exploded;

public:
    Bomb(Position _p = {-1, -1}, int _range = DEFAULT_RANGE, double _timer = DEFAULT_TIMER);

    Position get_position();

    bool is_exploded();

    bool is_timer_finished(double game_timer);

    void place(Position _p, double _game_timer, int _range = DEFAULT_RANGE, double _timer = DEFAULT_TIMER);

    void explode(Map& map);

    void reset();
};

#endif