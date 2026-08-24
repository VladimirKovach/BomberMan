#ifndef CHASER_HPP
#define CHASER_HPP

#include "enemy.hpp"
#include "map.hpp"
#include "utils.hpp"

class Chaser : public Enemy {
private:
    int distance(Position a, Position b);

    void sort_directions(Position player_p);

    void move(Map& map, Position player_p);

public:
    Chaser(Position _p = {-1, -1}, int _speed = 1);

    void update(Map& map, Position player_p);
};

#endif