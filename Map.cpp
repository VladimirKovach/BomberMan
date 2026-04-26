#include "Map.hpp"
#include <cstdlib>

bool positions_equal(Position p1, Position p2) {
    return p1.x == p2.x && p1.y == p2.y;
}


Map::Map(int difficulty) {
    // Inizializza tutta la griglia come EMPTY
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            grid[y][x] = EMPTY;
        }
    }

    // Bordi della mappa (muri indistruttibili)
    for (int x = 0; x < MAP_COLS; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[MAP_ROWS - 1][x] = UNBREAKABLE_WALL;
    }
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][MAP_COLS - 1] = UNBREAKABLE_WALL;
    }

    // Muri indistruttibili interni (pattern a scacchiera)
    place_unbreakable_walls();

    // Muri distruttibili (quantita' basata sulla difficolta')
    place_breakable_walls(difficulty);

    // Riempi l'array empty_cells con tutte le celle rimaste EMPTY
    empty_cells_count = 0;
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY) {
                add_empty_cell({x, y});
            }
        }
    }
}


// =====================================================
// Controlla se la cella (x,y) NON deve mai ricevere muri distruttibili
// Protegge solo la zona spawn del giocatore: (1,1), (2,1), (1,2)
// Senza questo, il giocatore potrebbe iniziare intrappolato.
// =====================================================
bool Map::is_safe_zone(int x, int y) {
    if (x == 1 && y == 1) {
        return true;
    }
    else if (x == 2 && y == 1) {
        return true;
    }
    else if (x == 1 && y == 2) {
        return true;
    }
    else {
        return false;
    }
}


void Map::place_unbreakable_walls() {
    for (int y = 1; y < MAP_ROWS - 1; y += 2) {
        for (int x = 1; x < MAP_COLS - 1; x += 2) {
            int place = rand() % 5;
            if (place == 1) {
                grid[y][x] = UNBREAKABLE_WALL;
            }
        }
    }
}


void Map::place_breakable_walls(int difficulty) {
    if (difficulty < 1) {
        difficulty = 1;
    }
    else if (difficulty > MAX_DIFFICULTY) {
        difficulty = MAX_DIFFICULTY;
    }

    int percentage[MAX_DIFFICULTY] = {5, 10, 15, 20, 25};

    int empty_cells = 0;
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY && !is_safe_zone(x, y)) {
                empty_cells++;
            }
        }
    }

    int walls_to_place = empty_cells * percentage[difficulty - 1] / 100;

    int placed = 0;
    int max_attempts = walls_to_place * 10;
    int attempts = 0;

    while (placed < walls_to_place && attempts < max_attempts) {
        int x = 1 + rand() % (MAP_COLS - 2);
        int y = 1 + rand() % (MAP_ROWS - 2);

        if (grid[y][x] == EMPTY && !is_safe_zone(x, y)) {
            grid[y][x] = BREAKABLE_WALL;
            placed++;
        }
        attempts++;
    }
}


bool Map::in_bounds(Position p) {
    return (p.x >= 0 && p.x < MAP_COLS) && (p.y >= 0 && p.y < MAP_ROWS);
}

bool Map::is_empty_cell(Position p) {
    return grid[p.y][p.x] == EMPTY;
}

bool Map::is_wall(Position p) {
    return in_bounds(p) &&
           (grid[p.y][p.x] == BREAKABLE_WALL || 
           grid[p.y][p.x] == UNBREAKABLE_WALL);
}

bool Map::is_door(Position p) {
    return in_bounds(p) &&
           (grid[p.y][p.x] == PREV_DOOR || 
           grid[p.y][p.x] == NEXT_DOOR);
}

bool Map::is_enemy(Position p) {
    return in_bounds(p) &&
           (grid[p.y][p.x] == DUMMY_ENEMY ||
           grid[p.y][p.x] == SMART_ENEMY);
}

bool Map::is_explosion(Position p) {
    return in_bounds(p) && grid[p.y][p.x] == EXPLOSION;
}

void Map::add_empty_cell(Position p) {
    if (is_empty_cell(p)) {
        empty_cells[empty_cells_count] = p;
        empty_cells_count++;
    }
}


void Map::remove_empty_cell(Position p) {
    bool found = false;
    int i = 0;

    while (i < empty_cells_count && !found) {
        if (positions_equal(empty_cells[i], p)) {
            found = true;

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


// da modificare: le celle (1, 2), (2, 1) non vanno bene all'inizio
Position Map::get_random_empty_cell() {
    if (empty_cells_count != 0) {
        int i = rand() % empty_cells_count;
        return empty_cells[i];
    }
    else {
        return {-1, -1};  // da gestire
    }
}

CellContent Map::get_cell_content(Position p) {
    if (in_bounds(p)) {
        return grid[p.y][p.x];
    }
    else {
        return UNKNOWN;
    }
}

void Map::set_cell_content(Position p, CellContent content) {
    if (in_bounds(p)) {
        if (is_empty_cell(p)) {
            remove_empty_cell(p);
        }
        grid[p.y][p.x] = content;
    }
}

void Map::clear_cell(Position p) {
    if (in_bounds(p) && !is_empty_cell(p)) {
        grid[p.y][p.x] = EMPTY;
        add_empty_cell(p);
    }
}


// =====================================================
// Porte tra livelli
// NEXT_DOOR (uscita): bordo superiore destro
// PREV_DOOR (entrata): bordo superiore sinistro
// =====================================================
void Map::open_next_door() {
    grid[1][MAP_COLS - 1] = NEXT_DOOR;
}

void Map::open_prev_door() {
    grid[1][0] = PREV_DOOR;
}

void Map::close_next_door() {
    grid[1][MAP_COLS - 1] = UNBREAKABLE_WALL;
}

void Map::close_prev_door() {
    grid[1][0] = UNBREAKABLE_WALL;
}