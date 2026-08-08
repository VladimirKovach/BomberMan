#ifndef CHASER_HPP
#define CHASER_HPP

#include "enemy.hpp"

class Chaser : public Enemy {
private:
    void move(Map& map);

public:
    Chaser(Position p = {-1, -1}, int speed = 1);

    void update(Map& map, Position player_p);
};

#endif