#ifndef ROAMER_HPP
#define ROAMER_HPP

#include "enemy.hpp"

class Roamer : public Enemy {
private:
    void move(Map& map);

public:
    Roamer(Position p = {-1, -1}, int speed = 1);

    void update(Map& map);
};

#endif