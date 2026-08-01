#ifndef ITEM_HPP
#define ITEM_HPP

#include "map.hpp"
#include "utils.hpp"

// Tipi di item raccoglibili dal giocatore
enum ItemType {
    ITEM_RANGE,  // aumenta temporaneamente il raggio delle bombe
    ITEM_LIFE,   // aggiunge una vita (effetto istantaneo)
    ITEM_TIME,   // aggiunge tempo al timer (effetto istantaneo)
    ITEM_SCORE,  // aggiunge punti al punteggio (effetto istantaneo)
};

// Parametri degli effetti
const int RANGE_BONUS = 1;                                  // raggio aggiuntivo durante il buff
const int RANGE_DURATION_SHORT = 5 * TICKS_PER_SECOND;      // 5 s  = 100 tick
const int RANGE_DURATION_LONG = 10 * TICKS_PER_SECOND;      // 10 s = 200 tick
const int TIME_BONUS = 30 * TICKS_PER_SECOND;               // 30 s = 600 tick
const int SCORE_BONUS = 4;                                  // punti

class Item {
protected:
    Position p;     // posizione sulla griglia
    ItemType type;  // tipo di effetto
    int duration;   // durata del buff in tick (usata solo da ITEM_RANGE)
    bool active;    // true se l'item e' ancora a terra (non raccolto)

public:
    Item(Position _p = {-1, -1}, ItemType _type = ITEM_RANGE);

    Position get_position();
    ItemType get_type();
    int get_duration();
    bool is_active();

    // Posiziona/attiva l'item sul terreno; per ITEM_RANGE sorteggia la durata
    void spawn(Position _p, ItemType _type);

    // Raccolta: disattiva l'item
    void collect();

    void reset();
};

#endif