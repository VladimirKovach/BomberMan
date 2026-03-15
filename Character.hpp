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
    Position pos;
    int lives;
    int speed;  // cell per move

public:
    Character(Position _pos, int _lives, int _speed);

    bool is_dead();

    Position get_position();

    Position get_next_position(Direction dir);

    void move(Map& map, Direction dir);
};

#endif