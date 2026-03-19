#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class Enemy : public Character {
protected:
    int last_move_time;

    Direction choose_direction();

public:
    Enemy(Position _pos, int _lives, int _speed);

    void move(Map& map, int timer);
};

#endif