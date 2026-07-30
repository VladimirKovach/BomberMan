#ifndef SMARTENEMY_HPP
#define SMARTENEMY_HPP

#include "dummy_enemy.hpp"
#include "map.hpp"

class SmartEnemy: public DummyEnemy {
protected:
    Position player_p;

    void update_player_position(Position _player_p);
    int get_player_distance(Position _p);

    void plan_move();

    // duplicati di DummyEnemy (non posso usare virtual e override)
    bool is_valid_move(Map& map, Position _p);
    void move(Map& map);

public:
    SmartEnemy(Position _p = {-1, -1}, int _speed = 1);

    void update(Map& map, Position _player_p);
};

#endif