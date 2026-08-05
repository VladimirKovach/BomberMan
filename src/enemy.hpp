#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "utils.hpp"

const int DIRECTIONS_COUNT = 4;

class Enemy {
protected:
    Direction _directions[DIRECTIONS_COUNT];

    Position _p;
    Position _start_p;

    int _speed;
    int _start_speed;
    int _move_timer;

    bool _dead;

public:
    Enemy(Position p = {-1, -1}, int speed = 1);

    Position get_position();
    void set_position(Position p);

    int get_speed();
    void set_speed(int speed);

    void kill();
    bool is_dead();

    void shuffle_directions();

    bool can_move(Map& map, Direction d);

    void reset();
};

#endif