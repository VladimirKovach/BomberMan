#ifndef BOMB_HPP
#define BOMB_HPP

#include "Map.hpp"

const int MAX_BOMBS = 10;           // max bombe piazzabili contemporaneamente
const int BOMB_DEFAULT_TIMER = 180; // frame prima dell'esplosione (~3 sec a 60fps)
const int EXPLOSION_DURATION = 30;  // frame di persistenza del colore esplosione (~0.5 sec)

class Bomb {
protected:
    Position p;
    int range;
    int timer;
    bool active;
    bool exploded;

public:
    Bomb();
    Bomb(Position _p, int _range, int _timer);

    void place(Position _p, int _range, int _timer);
    Position get_position();
    int get_range();
    bool is_active();
    bool has_exploded();
    bool is_timer_expired();
    void update_timer();
    void explode(Map& map);  // esegue l'esplosione sulla mappa
    void reset();            // resetta la bomba per riuso
};

#endif