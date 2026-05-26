#ifndef BOMB_HPP
#define BOMB_HPP

#include "Grid.hpp"
#include "Player.hpp"  // da cambiare in Player.hpp

const double EXPLOSION_TIMER = 3.0;  // tempo che la bomba impiega a esplodere
const double EXPLOSION_DURATION = 2.0;  // tempo di persistenza dell'esplosione

// intervallo di tempo tra due lampeggi consecutivi della bomba
const double BLINK_DELTA = 0.5;

class Bomb {
protected:
    Position p;
    int range;

    bool active;
    bool exploding;
    bool blink_state;

    double placement_time;
    double explosion_time;

    // momento di inizio dello stato (lampeggio/non lampeggio)
    double blink_state_start;

    void begin_explosion(Grid& grid, Direction d);
    void end_explosion(Grid& grid, Direction d);

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();

    bool is_active();
    bool is_exploding();
    bool is_blinking();

    void place(Position _p, int _range, double game_timer);

    void explode(Grid& grid, double game_timer);

    void update(Grid& grid, double game_timer);

    void reset();
};

#endif