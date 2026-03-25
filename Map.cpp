#include "Map.hpp"
#include <cstdlib>

bool positions_equal(Position p, Position q) {
    return (p.x == q.x && p.y == q.y);
}


Map::Map(int difficulty) {
    // PASSO 1: Inizializza tuttas la griglia come EMPTY
    empty_cells_count = 0;

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            grid[y][x] = EMPTY;
        }
    }

    // PASSO 2: Bordi della mappa (muri indistruttibili)
    // Riga superiore e inferiore
    for (int x = 0; x < MAP_COLS; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[MAP_ROWS - 1][x] = UNBREAKABLE_WALL;
    }

    // Colonna sinistra e destra
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][MAP_COLS - 1] = UNBREAKABLE_WALL;
    }

    // PASSO 3: Muri indistruttibili interni (pattern a scacchiera)
    place_unbreakable_walls();

    // PASSO 4: Muri distruttibili (quantita' basata sulla difficolta')
    place_breakable_walls(difficulty);

    // PASSO 5: Riampiamo l'array empty_cells con tutte le celle rimaste EMPTY
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY) {
                add_empty_cell({x, y});
            }
        }
    }
}


// =====================================================
// is_safe_zone: controlla se la cella (x,y) e' troppo
// vicina allo spawn del giocatore (posizione 1,1).
//
// Le celle protette sono:
//   (1,1)  (2,1)  (1,2)
//
// Senza questa protezione, il giocatore potrebbe
// iniziare circondato da muri, senza la possibilita'
// di uscire senza farsi male.
// =====================================================
bool Map::is_safe_zone(int x, int y) {
    // Cella dello spawn
    if (x == 1 && y == 1) return true;

    // Cella a destra dello spawn
    if (x == 2 && y == 1) return true;

    // Cella sotto lo spawn
    if (x == 1 && y == 2) return true;

    return false;
}


// =====================================================
// place_unbreakable_walls: piazza il pattern FISSO
// di muri indistruttibili all'interno della mappa.
//
// La regola e': se ENTRAMBE le coordinate (riga e colonna)
// sono numeri PARI, allora la cella e' un muro indistruttibile.
//
// Esempio su griglia piccola (. = vuoto, # = indistruttibile):
//
//   col:  0  1  2  3  4  5  6
//  rig 0: #  #  #  #  #  #  #    <- bordo
//  rig 1: #  .  .  .  .  .  #
//  rig 2: #  .  #  .  #  .  #    <- riga pari: muri a colonne pari
//  rig 3: #  .  .  .  .  .  #
//  rig 4: #  .  #  .  #  .  #    <- riga pari: muri a colonne pari
//  rig 5: #  .  .  .  .  .  #
//  rig 6: #  #  #  #  #  #  #    <- bordo
//
// Questo pattern e' lo STESSO in tutti i livelli.
// =====================================================
void Map::place_unbreakable_walls() {
    for (int y = 2; y < MAP_ROWS - 1; y += 2) {
        for (int x = 2; x < MAP_COLS - 1; x += 2) {
            grid[y][x] = UNBREAKABLE_WALL;
        }
    }
}


// =====================================================
// place_breakable_walls: piazza i muri distruttibili
// in posizioni casuali sulla mappa.
//
// La quantita' dipende dal parametro 'difficulty':
//   - difficulty 1 (livello 1): 30% delle celle interne vuote
//   - difficulty 2 (livello 2): 37% delle celle interne vuote
//   - difficulty 3 (livello 3): 45% delle celle interne vuote
//   - difficulty 4 (livello 4): 52% delle celle interne vuote
//   - difficulty 5 (livello 5): 60% delle celle interne vuote
//
// Una cella puo' diventare muro distruttibile SOLO se:
//   -> E' attualmente EMPTY (non e' gia' un muro indistruttibile)
//   -> NON e' nella zona sicura intorno allo spawn del giocatore
//
// Come funziona il calcolo:
//   1) Contiamo quante celle interne sono vuote DOPO aver piazzato i muri indistruttibili
//   2) Calcoliamo quante di queste riempire in base alla %
//   3) Le piazziamo in posizioni casuali (usando rand())
// =====================================================
void Map::place_breakable_walls(int difficulty) {
    // Tabella delle percentuali di riempimento per ogni livello.
	// Indice 0 non si usa.
    // Difficulty parte da 1, in totale sono 5 livelli.
    //
    // percentage[1] = 30  significa  30%  delle celle vuote
    // percentage[5] = 60  significa  60%  delle celle vuote
    int percentage[6] = {0, 30, 37, 45, 52, 60};

    // FASE 1: Conta quante celle interne sono EMPTY
    // (cioe' celle che NON sono bordi e NON sono muri indistruttibili)
    int total_empty = 0;
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY && !is_safe_zone(x, y)) {
                total_empty++;
            }
        }
    }

    // FASE 2: Calcola quanti muri distruttibili piazzare
    // Esempio: se total_empty = 500 e percentage = 30
    //          walls_to_place = 500 * 30 / 100 = 150
    int walls_to_place = total_empty * percentage[difficulty] / 100;

    // FASE 3: Piazza i muri in posizioni casuali
    // Strategia: scegliamo coordinate casuali (x,y) e se la cella
    // e' vuota e non e' nella zona sicura, ci mettiamo un muro.
    // Se la cella non va bene, riproviamo.
    int placed = 0;
    int max_attempts = walls_to_place * 10;  // limite anti-loop infinito
    int attempts = 0;

    while (placed < walls_to_place && attempts < max_attempts) {
        // Genera coordinate casuali nell'area interna
        // (da 1 a MAP_COLS-2 per x, da 1 a MAP_ROWS-2 per y)
        int x = 1 + rand() % (MAP_COLS - 2);
        int y = 1 + rand() % (MAP_ROWS - 2);

        // Controlla se la cella e' adatta
        if (grid[y][x] == EMPTY && !is_safe_zone(x, y)) {
            grid[y][x] = BREAKABLE_WALL;
            placed++;
        }

        attempts++;
    }
}


bool Map::cell_exists(Position pos) {
    return (
        pos.x >= 0 && pos.x < MAP_COLS &&
        pos.y >= 0 && pos.y < MAP_ROWS
    );
}

bool Map::is_empty_cell(Position pos) {
    return grid[pos.y][pos.x] == EMPTY;
}

bool Map::is_walkable_cell(Position pos) {
    return (
        cell_exists(pos) &&
        grid[pos.y][pos.x] != BREAKABLE_WALL &&
        grid[pos.y][pos.x] != UNBREAKABLE_WALL
    );
}

void Map::add_empty_cell(Position pos) {
    if (is_empty_cell(pos)) {
        empty_cells[empty_cells_count] = pos;
        empty_cells_count++;
    }
}


void Map::remove_empty_cell(Position pos) {
    bool found = false;
    int i = 0;

    while (i < empty_cells_count && !found) {
        if (positions_equal(empty_cells[i], pos)) {
            found = true;

            // shift left all the elements at the right of the removed one
            for (i; i < empty_cells_count - 1; i++) {
                empty_cells[i] = empty_cells[i + 1];
            }

            empty_cells_count--;
        }

        else {
            i++;
        }
    }
}


Position Map::get_random_empty_cell() {
    if (empty_cells_count != 0) {
        int i = rand() % empty_cells_count;
        return empty_cells[i];
    }
    else {
        return {-1, -1}; // da gestire
    }
}

CellContent Map::get_cell_content(Position pos) {
    if (cell_exists(pos)) {
        return grid[pos.y][pos.x];
    }
    else {
        return UNKNOWN;
    }
}

void Map::set_cell_content(Position pos, CellContent content) {
    if (cell_exists(pos) && is_empty_cell(pos)) {
        grid[pos.y][pos.x] = content;
        remove_empty_cell(pos);
    }
}

void Map::clear_cell(Position pos) {
    if (cell_exists(pos) && !is_empty_cell(pos)) {
        grid[pos.y][pos.x] = EMPTY;
        add_empty_cell(pos);
    }
}

void Map::open_next_level_door() {
    clear_cell({MAP_COLS - 1, 1});
}