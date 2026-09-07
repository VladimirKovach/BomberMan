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

    int bomb_range;
    int buff_timer;

public:
    Player(Position _p = {1, 1}, int _lives = 3);

    Position get_position();

    // Imposta una nuova posizione corrente e iniziale
    // Va chiamata all'ingresso in un livello,
    // cosi' alla morte si riparte dalla porta da cui si è entrati
    void spawn(Position _p);

    int get_lives();
    void gain_life();
    void lose_life();
    bool is_dead();

    bool can_move_to(Map& map, Position next);
    void move(Map& map, Direction d);

    int get_bomb_range();

    // Attiva (o prolunga) il buff raggio per 'duration' tick
    void apply_buff(int duration);

    // Scala il timer del buff raggio di un tick e lo disattiva alla scadenza
    // Va chiamata a ogni frame
    void update_buff();

    // Tick rimanenti del buff raggio (0 se non attivo)
    int get_buff_remaining();

    void reset();
};

#endif