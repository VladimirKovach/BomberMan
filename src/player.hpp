#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "map.hpp"
#include "utils.hpp"
#include "item.hpp"

const int MAX_LIVES = 3;
const int MAX_BOMB_SLOTS = 3;

class Player {
protected:
    Position _p;
    Position _start_p;

    int _lives;

    int _bomb_slots;
    int _bomb_range;   // raggio corrente delle bombe (1 = base)
    int _buff_timer;   // tick rimanenti del buff raggio (0 = nessun buff)

public:
    Player(Position p = {1, 1}, int lives = 3);

    Position get_position();
    void set_position(Position p);

    int get_lives();
    void gain_life();
    void lose_life();
    bool is_dead();

    bool can_move_to(Map& map, Position p);
    void move(Map& map, Direction d);

    int get_bomb_slots();
    void increase_bomb_slots();

    int get_bomb_range();

    // Attiva o prolunga il buff raggio per 'duration' tick
    void apply_range_buff(int duration);

    // Scala il buff di un tick e lo disattiva alla scadenza (ogni frame)
    void update_buff();

    // Tick rimanenti del buff (0 se non attivo)
    int get_buff_remaining();

    void reset();
};

#endif