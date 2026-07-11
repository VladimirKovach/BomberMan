#ifndef ITEM_HPP
#define ITEM_HPP

#include "Grid.hpp"

// Tipi di item raccoglibili dal giocatore
enum ItemType {
    ITEM_RANGE,  // aumenta temporaneamente il raggio delle bombe
    ITEM_LIFE,   // aggiunge una vita (effetto istantaneo)
    ITEM_TIME,   // aggiunge tempo al timer (effetto istantaneo)
    ITEM_SCORE,  // aggiunge punti al punteggio (effetto istantaneo)
};

// Parametri degli effetti
const int RANGE_BONUS = 2;                 // raggio aggiuntivo durante il buff
const double RANGE_DURATION_SHORT = 5.0;   // durata corta del buff (secondi)
const double RANGE_DURATION_LONG = 10.0;   // durata lunga del buff (secondi)
const int TIME_BONUS = 30;                 // secondi aggiunti dall'item tempo
const int SCORE_BONUS = 4;                 // punti aggiunti dall'item punteggio

class Item {
protected:
    Position p;       // posizione sulla griglia
    ItemType type;    // tipo di effetto
    double duration;  // durata del buff (usata solo da ITEM_RANGE)
    bool active;      // true se l'item e' ancora a terra (non raccolto)

public:
    Item(Position _p = {-1, -1}, ItemType _type = ITEM_RANGE);

    Position get_position();
    ItemType get_type();
    double get_duration();
    bool is_active();

    // Posiziona/attiva l'item sul terreno; per ITEM_RANGE sorteggia la durata
    void spawn(Position _p, ItemType _type);

    // Raccolta: disattiva l'item
    void collect();

    void reset();
};

#endif