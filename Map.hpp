#ifndef MAP_HPP
#define MAP_HPP

#include "Level.hpp"

const int NUM_LEVELS = 5;

struct Node {
    Level level;
    Node* next;
    Node* prev;
};

class Map {
protected:
    Node* head;
    Node* current;

public:
    Map();
    ~Map();

    Level& get_current_level();

    bool has_next_level();
    bool has_prev_level();

    void go_to_next_level();
    void go_to_prev_level();

    // Rimuove dalla lista il nodo corrente e sposta 'current'
    // al nodo adiacente (next se forward=true, prev se forward=false).
    // Aggiorna 'head' se necessario.
    // Restituisce false se non c'e' un nodo adiacente nella direzione richiesta.
    bool remove_current_level(bool forward);

    bool is_current_completed();
    bool all_levels_completed();

    void update_doors();
    void update_all_bombs(double game_timer);
};

#endif