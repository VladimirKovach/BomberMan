#ifndef WALKER_HPP
#define WALKER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "utils.hpp"

class Walker : public Enemy {
private:
    Direction direction;

    void move(Map& map);

public:
    Walker(Position _p = {-1, -1}, int _speed = 1);

    void update(Map& map);
};

#endif