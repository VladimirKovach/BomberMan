#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
protected:

public:
    // Set to upper-left cell (1, 1)
    Player(Position _p = {1, 1}, int _lives = 1);

    
};

#endif