#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "map.hpp"
#include "position.hpp"

const int MAX_LIVES = 3;

class Player {
protected:
    Position p;
    Position start_p;

    int lives;

    int bomb_range;  // raggio corrente delle bombe (1 se base)
    int buff_timer;  // tick rimanenti del buff raggio (0 se non attivo)

public:
    Player(Position _p = {1, 1}, int _lives = 3);

    Position get_position();
    void set_position(Position _p);

    // Va usata all'ingresso in un livello,
    // cosi' alla morte si riparte dalla porta da cui si è entrati.
    void set_spawn(Position _p);

    int get_lives();
    void gain_life();
    void lose_life();
    bool is_dead();

    bool can_move_to(Map& map, Position next);
    void move(Map& map, Direction d);

    int get_bomb_range();

    // Attiva (o prolunga) il buff raggio per 'duration' tick
    void apply_buff(int duration);

    // Scala il buff raggio di un tick e lo disattiva alla scadenza (ogni frame)
    void update_buff();

    // Tick rimanenti del buff raggio (0 se non attivo)
    int get_buff_remaining();

    void reset();
};

#endif