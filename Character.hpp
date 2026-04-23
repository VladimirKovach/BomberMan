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
    CellContent type;
    CellContent under;  // EMPTY, BOMB, ITEM

public:
    Character(Position _p = {0, 0}, int _lives = 1, CellContent _type = PLAYER);

    void take_damage();

    bool is_dead();

    Position get_position();

    Position get_next_position(Direction d);

    CellContent get_under();

    void move(Map& map, Direction d);
};

#endif