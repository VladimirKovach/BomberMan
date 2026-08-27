#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "utils.hpp"

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
    Enemy(Position _p = {-1, -1}, int _speed = 1);

    Position get_position();
    void set_position(Position _p);

    int get_speed();

    void kill();
    bool is_dead();

    void reset();
};

#endif