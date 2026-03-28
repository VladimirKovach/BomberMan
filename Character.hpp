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
    CellContent cell_under;  // la cella che c'è sotto al personaggio
    CellContent my_type;     // PLAYER o ENEMY

public:
    Character(Position _p = {0, 0}, int _lives = 1, int _speed = 1, CellContent _type = PLAYER);

    void gain_life();
    void lose_life();
    bool is_dead();

    Position get_position();
    Position get_next_position(Direction d);
    void set_cell_under(CellContent content);
    void move(Map& map, Direction d);
};

#endif