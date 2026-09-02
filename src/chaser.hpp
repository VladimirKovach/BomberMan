#ifndef CHASER_HPP
#define CHASER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "position.hpp"

class Chaser : public Enemy {
protected:
    int distance(Position a, Position b);

    void sort_directions(Position player_p);

    void move(Map& map, Position player_p);

public:
    Chaser(Position _p = POSITION_NONE, int _speed = 2);

    void update(Map& map, Position player_p);
};

#endif