#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Map.hpp"

const int NEIGHBORS_COUNT = 4;

class DummyEnemy {
protected:
    Position p;
    Position start_p;

    int speed;
    int start_speed;
    double move_timer;
    double last_move_time;

    bool dead;

    Position neighbors[NEIGHBORS_COUNT];

    void update_neighbors();

    bool can_move(double game_timer);
    void plan_move();

    bool is_valid_move(Map& map, Position _p);
    void move(Map& map, double game_timer);

public:
    DummyEnemy(Position _p = {-1, -1}, int _speed = 0);

    Position get_position();
    void set_position(Position _p);

    int get_speed();

    void kill();
    bool is_dead();

    void update(Map& map, double game_timer);

    void reset();
};

#endif