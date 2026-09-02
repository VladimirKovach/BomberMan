#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "position.hpp"

class Enemy {
protected:
    Direction directions[DIRECTIONS_COUNT];

    Position p;
    Position start_p;

    int speed;
    int move_timer;

    bool dead;

    void shuffle_directions();

    bool can_move_to(Map& map, Position next);

public:
    Enemy(Position _p = POSITION_NONE, int _speed = 1);

    Position get_position();
    int get_speed();

    void kill();
    bool is_dead();

    void reset();
};

#endif