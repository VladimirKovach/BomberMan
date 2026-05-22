#ifndef SMARTENEMY_HPP
#define SMARTENEMY_HPP

#include "Character.hpp"
#include "DummyEnemy.hpp"

class SmartEnemy: public DummyEnemy {
protected:
    Position player_p;

    void update_player_position(Position _player_p);
    double get_player_distance(Position _p);

    void plan_move();

    // duplicati di DummyEnemy (non posso usare virtual e override)
    bool is_valid_move(Grid& grid, Position _p);
    void move(Grid& grid, double game_timer);

public:
    SmartEnemy(Position _p = {-1, -1}, int _lives = 0, int _speed = 0);

    void update(Grid& grid, double game_timer, Position _player_p);
};

#endif