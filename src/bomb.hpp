#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "player.hpp"
#include "utils.hpp"

const int BLINKING_TIMER_START = TICKS_PER_SECOND / 4;   // 0.25 secondi
const int EXPLODING_TIMER_START = TICKS_PER_SECOND * 2;  // 2.00 secondi
const int EXPLOSION_TIMER_START = TICKS_PER_SECOND / 2;  // 0.50 secondi

class Bomb {
private:
    Position _p;
    int _range;

    bool _active;
    bool _blinking;
    bool _exploding;

    int _blinking_timer;
    int _exploding_timer;
    int _explosion_timer;

    void update_explosion(Map& map, Direction d, bool set);

public:
    Bomb(Position p = {-1, -1}, int range = 1);

    Position get_position();
    int get_range();

    bool is_active();
    bool is_blinking();
    bool is_exploding();

    void place(Map& map, Position p, int range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif