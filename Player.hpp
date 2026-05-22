#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Grid.hpp"

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE,
};

Position get_next_position(Position p, Direction d);

class Player : public Character {
public:
    Player(Position _p = {1, 1}, int _lives = 3);

    void heal();

    void move(Grid& grid, Direction d);
};

#endif