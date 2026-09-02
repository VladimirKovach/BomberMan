#ifndef ROAMER_HPP
#define ROAMER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "position.hpp"

class Roamer : public Enemy {
protected:
    void move(Map& map);

public:
    Roamer(Position _p = POSITION_NONE, int _speed = 1);

    void update(Map& map);
};

#endif