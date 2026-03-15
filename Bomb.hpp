#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

class Bomb {
protected:
    Position pos;
    int range;
    int timer;

public:
    Bomb(Position _pos, int _range = 1, int _timer = 5);

    Position get_position();

    bool is_timer_expired();

    void update_timer();

    void explode();
};

#endif