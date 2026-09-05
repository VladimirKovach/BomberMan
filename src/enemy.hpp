#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "map.hpp"
#include "position.hpp"

// Tipi di nemico, usati per interrogare il livello sul contenuto di una cella
enum EnemyType {
    ENEMY_CHASER,
    ENEMY_ROAMER,
    ENEMY_WALKER,
};

class Enemy {
protected:
    Direction directions[DIRECTIONS_COUNT];

    Position p;
    Position start_p;

    int speed;
    int move_timer;

    bool dead;

    void shuffle_directions();

    bool can_move_to(Map& map, Position next);

public:
    Enemy(Position _p = POSITION_NONE, int _speed = 1);

    Position get_position();
    int get_speed();

    void kill();
    bool is_dead();

    void reset();
};

#endif