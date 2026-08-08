#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "utils.hpp"

enum EnemyType {
    WALKER,
    ROAMER,
    CHASER,
};

const int DIRECTIONS_COUNT = 4;

class Enemy {
protected:
    Direction _directions[DIRECTIONS_COUNT];

    Position _p;
    Position _start_p;

    int _speed;
    int _start_speed;
    int _move_timer;

    bool _dead;

    EnemyType _type;

    Direction _direction;  // per walker

    int distance(Position a, Position b);
    void shuffle_directions();
    void sort_directions(Position player_p);

    bool can_move_to(Map& map, Position p);
    void move_walker(Map& map);
    void move_roamer(Map& map);
    void move_chaser(Map& map, Position player_p);
    void move(Map& map, Position player_p);

public:
    Enemy(Position p = {-1, -1}, int speed = 1, EnemyType type = WALKER);

    Position get_position();
    void set_position(Position p);

    int get_speed();

    void kill();
    bool is_dead();

    EnemyType get_type();

    void update(Map& map, Position player_p);

    void reset();
};

#endif