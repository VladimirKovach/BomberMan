#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "Map.hpp"

// =====================================================
// LevelNode: un nodo della lista bidirezionale.
//
// Ogni nodo rappresenta un livello del gioco e contiene:
//   - map: la mappa del livello (con tutto il suo stato)
//   - level_number: numero del livello (1, 2, 3, 4, 5)
//   - difficulty: difficolta' del livello (uguale a level_number)
//   - completed: true se tutti i nemici sono stati sconfitti
//   - next: puntatore al livello successivo (NULL se ultimo)
//   - prev: puntatore al livello precedente (NULL se primo)
// =====================================================
struct LevelNode {
    Map map;
    int level_number;
    int difficulty;
    bool completed;
    LevelNode* next;
    LevelNode* prev;
};


// =====================================================
// LevelManager: gestisce la lista bidirezionale di livelli.
//
// Il costruttore crea NUM_LEVELS nodi, ognuno con
// difficolta' crescente, e li collega tra loro.
//
// Il puntatore 'current' indica il livello attivo.
// Il Game usa get_current_map() per ottenere la mappa su cui il giocatore sta giocando.
// =====================================================
const int NUM_LEVELS = 5;

class LevelManager {
protected:
    LevelNode* head;        // primo livello della lista
    LevelNode* current;     // livello attualmente in gioco

public:
    // Crea NUM_LEVELS nodi collegati, con difficolta' 1..5
    LevelManager();

    // Dealloca tutti i nodi della lista con delete
    ~LevelManager();

    // Restituisce un riferimento alla mappa del livello corrente.
    Map& get_current_map();

    int get_current_level_number();

    int get_current_difficulty();

    bool has_next_level();

    bool has_prev_level();

    bool go_to_next_level();

    bool go_to_previous_level();

    // Segna il livello corrente come completato
    void mark_current_completed();

    bool is_current_completed();

    bool all_levels_completed();

    // Aggiorna le porte del livello corrente:
    // - Se tutti i nemici sono morti E c'e' un livello successivo -> apre DOOR_NEXT
    // - Se c'e' un livello precedente -> apre DOOR_PREV
    // - Altrimenti chiude le porte
    void update_doors(int enemy_count);
};

#endif