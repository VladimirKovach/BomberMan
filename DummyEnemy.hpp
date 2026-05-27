#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

const int ADJACENT_POSITIONS_COUNT = 4;

class DummyEnemy : public Character {
protected:
    int speed;
    double move_timer;
    double last_move_time;
    Position start_p;
    int start_lives;
    int start_speed;

    Position adjacent_positions[ADJACENT_POSITIONS_COUNT];

    void update_adjacent_positions();

    bool can_move(double game_timer);
    void plan_move();

    bool is_valid_move(Grid& grid, Position _p);
    void move(Grid& grid, double game_timer);

public:
    DummyEnemy(Position _p = {-1, -1}, int _lives = 0, int _speed = 0);

    void update(Grid& grid, double game_timer);

    void reset();
};

#endif