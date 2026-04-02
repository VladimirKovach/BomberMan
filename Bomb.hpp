#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

const int BOMB_DEFAULT_TIMER = 90;  // frame prima dell'esplosione
const int EXPLOSION_DURATION = 30;  // frame di persistenza del colore esplosione

class Bomb {
protected:
    Position p;
    int range;
    int timer;
    bool active;
    bool exploded;

public:
    Bomb();
    Bomb(Position _p, int _range, int _timer);

    Position get_position();

    int get_range();

    bool is_active();

    bool is_exploded();

    bool is_timer_expired();

    void update_timer();

    void place(Position _p, int _range, int _timer);

    void explode(Map& map);

    void reset();
};

#endif