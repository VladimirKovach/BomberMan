#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Grid.hpp"

class Character {
protected:
    Position p;
    int lives;

public:
    Character(Position _p, int _lives);

    void take_damage();

    bool is_dead();

    Position get_position();

    void set_position(Position _p);
};

#endif