#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Map.hpp"

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE
};

class Character {
protected:
    Position p;
    int lives;
    int speed;

public:
    Character(Position _p, int _lives, int _speed);

    void gain_life();

    void lose_life();

    bool is_dead();

    Position get_position();

    Position get_next_position(Direction d);

    void move(Map& map, Direction d);
};

#endif