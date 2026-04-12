#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
protected:

public:
    Player(Position _p = {1, 1}, int _lives = 1);

    void heal();
};

#endif