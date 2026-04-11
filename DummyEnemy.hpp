#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

const int DIRECTION_COUNT = 4;

class DummyEnemy : public Character {
protected:
    Direction directions[DIRECTION_COUNT];
    int speed;
    double move_timer;
    double last_move_time;

    void init_directions();
    virtual void choose_directions();

public:
    DummyEnemy(Position _p = {-1, -1}, int _lives = 1, int _speed = 1);

    void move(Map& map, double timer);
};

#endif