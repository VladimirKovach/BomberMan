#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
protected:
    bool can_place_bomb;

public:
    // Set to upper-left cell (1, 1)
    Player(Position _p = {1, 1}, int _lives = 1, int _speed = 1);

    void place_bomb(Map& map);
};

#endif