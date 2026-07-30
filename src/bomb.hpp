#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "player.hpp"

class Bomb {
protected:
    Position p;
    int range;

    bool active;
    bool exploding;
    bool blinking;

    int blink_timer;
    int explosion_timer;
    int explosion_duration;

    Position next_position(Position _p, Direction d);

    void start_explosion(Map& map, Direction d);
    void end_explosion(Map& map, Direction d);

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();

    bool is_active();
    bool is_exploding();
    bool is_blinking();

    void place(Position _p, int _range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif