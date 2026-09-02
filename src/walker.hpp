#ifndef WALKER_HPP
#define WALKER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "position.hpp"

class Walker : public Enemy {
protected:
    Direction direction;

    void move(Map& map);

public:
    Walker(Position _p = POSITION_NONE, int _speed = 1);

    void update(Map& map);
};

#endif