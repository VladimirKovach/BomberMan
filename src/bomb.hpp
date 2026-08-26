#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "utils.hpp"

const int EXPLODING_TIMER_START = TICKS_PER_SECOND * 2;  // 2.00 secondi
const int EXPLOSION_TIMER_START = TICKS_PER_SECOND / 2;  // 0.50 secondi

class Bomb {
protected:
    Position p;
    int range;

    bool active;
    bool exploding;

    int exploding_timer;
    int explosion_timer;

    // spostare in map?
    void update_explosion(Map& map, Direction d, bool set);

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();
    int get_range();

    bool is_active();
    bool is_exploding();

    void place(Map& map, Position _p, int _range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif