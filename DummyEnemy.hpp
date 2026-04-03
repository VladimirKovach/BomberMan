#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class DummyEnemy : public Character {
protected:
    int speed;
    int last_move_time;

    Direction choose_direction();

public:
    DummyEnemy(Position _p = {-1, -1}, int _lives = 1, int _speed = 1);

    void move(Map& map, int timer);
};

#endif