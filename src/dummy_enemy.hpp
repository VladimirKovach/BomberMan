#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "utils.hpp"

const int NEIGHBORS_COUNT = 4;

class DummyEnemy {
protected:
    Position p;
    Position start_p;

    int speed;
    int start_speed;
    int move_timer;

    bool dead;

    Position neighbors[NEIGHBORS_COUNT];

    void update_neighbors();

    void plan_move();
    bool is_valid_move(Map& map, Position _p);
    void move(Map& map);

public:
    DummyEnemy(Position _p = {-1, -1}, int _speed = 1);

    Position get_position();
    void set_position(Position _p);

    int get_speed();

    void kill();
    bool is_dead();

    void update(Map& map);

    void reset();
};

#endif