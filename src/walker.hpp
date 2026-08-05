#ifndef WALKER_HPP
#define WALKER_HPP

#include "enemy.hpp"

class Walker : public Enemy {
private:
    Direction _d;

    void move(Map& map);

public:
    Walker(Position p = {-1, -1}, int speed = 1);

    void update(Map& map);
};

#endif