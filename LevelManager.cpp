#include "LevelManager.hpp"


// =====================================================
// Costruttore: crea NUM_LEVELS nodi e li collega.
//
// Ogni livello ha difficolta' crescente (1, 2, 3, 4, 5).
// La difficolta' influenza:
//   - Quanti muri distruttibili ci sono (gestito da Map)
//   - Quanti nemici ci saranno (gestito da Game quando
//     spawna i nemici, usando get_current_difficulty())
//
// Dopo la creazione, la lista ha questa forma:
// =====================================================
LevelManager::LevelManager() {
    head = 0;
    current = 0;

    LevelNode* prev_node = 0;

    for (int i = 1; i <= NUM_LEVELS; i++) {
        LevelNode* node = new LevelNode;

        // Il costruttore di Map(difficulty) genera i muri automaticamente.
        node->map = Map(i);
        node->level_number = i;
        node->difficulty = i;
        node->completed = false;
        node->visited = false;
        node->next = 0;
        node->prev = prev_node;

        if (prev_node != 0) {
            prev_node->next = node;
        }
        else {
            head = node;
        }

        prev_node = node;
    }

    current = head;
}


// =====================================================
// Distruttore: scorre tutta la lista e fa delete di ogni nodo.
//
// Partiamo da head e andiamo avanti con next fino a NULL.
// Per ogni nodo, salviamo il puntatore next PRIMA di
// fare delete, altrimenti perdiamo il riferimento.
// =====================================================
LevelManager::~LevelManager() {
    LevelNode* node = head;
    while (node != 0) {
        LevelNode* next = node->next;
        delete node;
        node = next;
    }
    head = 0;
    current = 0;
}


Map& LevelManager::get_current_map() {
    return current->map;
}

int LevelManager::get_current_level_number() {
    return current->level_number;
}

int LevelManager::get_current_difficulty() {
    return current->difficulty;
}

bool LevelManager::has_next_level() {
    return (current != 0 && current->next != 0);
}

bool LevelManager::has_prev_level() {
    return (current != 0 && current->prev != 0);
}


// =====================================================
// go_to_next_level: sposta il puntatore current al nodo successivo
// Spostano il puntatore 'current' al nodo successivo
// o precedente nella lista. Non modificano nessuna mappa.
// =====================================================
bool LevelManager::go_to_next_level() {
    if (current != 0 && current->next != 0) {
        current = current->next;
        return true;
    }
    return false;
}


bool LevelManager::go_to_prev_level() {
    if (current != 0 && current->prev != 0) {
        current = current->prev;
        return true;
    }
    return false;
}


void LevelManager::mark_current_completed() {
    if (current != 0) {
        current->completed = true;
    }
}

void LevelManager::mark_current_visited() {
    if (current != 0) {
        current->visited = true;
    }
}

bool LevelManager::is_current_completed() {
    if (current != 0) {
        return current->completed;
    }
    return false;
}

bool LevelManager::is_current_visited() {
    if (current != 0) {
        return current->visited;
    }
    return false;
}


bool LevelManager::all_levels_completed() {
    LevelNode* node = head;
    while (node != 0) {
        if (!node->completed) {
            return false;
        }
        node = node->next;
    }
    return (head != 0);
}

// =====================================================
// update_doors: aggiorna le porte del livello corrente.
//
// Logica:
//   - DOOR_NEXT (bordo destro): si apre SOLO se tutti i
//     nemici del livello sono morti e esiste un livello
//     successivo. Questo obbliga il giocatore a completare
//     il livello prima di poter avanzare.
//
//   - DOOR_PREV (bordo sinistro): si apre SEMPRE se esiste
//     un livello precedente. Il giocatore puo' tornare
//     indietro in qualsiasi momento.
//
// Questa funzione viene chiamata ogni frame dal Game loop.
// =====================================================
void LevelManager::update_doors() {
    if (current == 0) return;

    Map& map = current->map;

    // Porta avanti: aperta solo se nemici tutti morti e c'e' un livello successivo
    if (is_current_completed() && current->next != 0) {
        map.open_next_door();
    }
    else {
        map.close_next_door();
    }

    // Porta indietro: aperta se c'e' un livello precedente
    if (current->prev != 0) {
        map.open_prev_door();
    }
    else {
        map.close_prev_door();
    }
}