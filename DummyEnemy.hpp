#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

const int DIRECTION_COUNT = 4;

class DummyEnemy : public Character {
protected:
    int speed;
    double move_timer;
    double last_move_time;

    Direction directions[DIRECTION_COUNT];

    void init_directions();

public:
    DummyEnemy(Position _p = {-1, -1}, int _lives = 1, int _speed = 1);

    bool can_move(double game_timer);

    void plan_movement();

    void move(Map& map, double game_timer);
};

#endif