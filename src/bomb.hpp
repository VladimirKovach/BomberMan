#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "player.hpp"
#include "utils.hpp"

const int BOMB_TIMER_START = 3 * TICKS_PER_SECOND;
const int EXPLOSION_TIMER_START = 2 * TICKS_PER_SECOND;
const int BLINK_TIMER_START = TICKS_PER_SECOND / 2;

class Bomb {
protected:
    Position p;
    int range;

    bool active;
    bool exploding;
    bool blinking;

    int bomb_timer;
    int explosion_timer;
    int blink_timer;

    void update_explosion(Map& map, Direction d, bool set);

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();

    bool is_active();
    bool is_exploding();
    bool is_blinking();

    void place(Map& map, Position _p, int _range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif