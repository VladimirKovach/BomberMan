#ifndef ROAMER_HPP
#define ROAMER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "utils.hpp"

class Roamer : public Enemy {
private:
    void move(Map& map);

public:
    Roamer(Position _p = {-1, -1}, int _speed = 1);

    void update(Map& map);
};

#endif