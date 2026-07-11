#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "Grid.hpp"
#include "Item.hpp"

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE,
};

Position get_next_position(Position p, Direction d);

class Player : public Character {
protected:
    Position start_p;

    int bomb_range;    // raggio corrente delle bombe (1 = base)
    double buff_end;   // valore dell'orologio di gioco a cui scade il buff raggio

public:
    Player(Position _p = {1, 1}, int _lives = 3);

    void heal();

    void move(Grid& grid, Direction d);

    int get_bomb_range();

    // Applica/estende il buff raggio. L'orologio di gioco game_clock e' decrescente.
    void apply_range_buff(double duration, double game_clock);

    // Disattiva il buff se scaduto. Da chiamare ad ogni frame.
    void update_buff(double game_clock);

    void reset();
};

#endif