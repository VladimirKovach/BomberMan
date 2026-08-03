#ifndef SMART_ENEMY_HPP
#define SMART_ENEMY_HPP

#include "dummy_enemy.hpp"
#include "map.hpp"
#include "utils.hpp"

class SmartEnemy: public DummyEnemy {
protected:
    int distance(Position a, Position b);

    void sort_neighbors(Position player_p);

    bool can_move(Map& map, Position _p);
    void move(Map& map);

public:
    SmartEnemy(Position _p = {-1, -1}, int _speed = 1);

    void update(Map& map, Position player_p);
};

#endif