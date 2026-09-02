#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "position.hpp"
#include "time.hpp"

const int BLINK_TIMER_START = TICKS_PER_SECOND / 4;      // 0.25 secondi
const int EXPLODING_TIMER_START = TICKS_PER_SECOND * 2;  // 2.00 secondi
const int EXPLOSION_TIMER_START = TICKS_PER_SECOND / 2;  // 0.50 secondi

class Bomb {
protected:
    Position p;
    int range;

    // Raggio effettivo dell'esplosione in ogni direzione
    int explosion_range[DIRECTIONS_COUNT];

    bool active;
    bool blink;
    bool exploding;

    int blink_timer;  // tick rimanenti all'inizio o alla fine del lampeggio
    int exploding_timer;
    int explosion_timer;

    void start_explosion(Map& map, Direction d);
    void stop_explosion(Map& map, Direction d);

public:
    Bomb();

    Position get_position();
    int get_range();

    bool is_active();
    bool is_blinking();
    bool is_exploding();

    void place(Map& map, Position _p, int _range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif