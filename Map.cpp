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

    spawn_count = 0;

    // secondo quadrante (primo quadrante escluso)
    for (int y = 0; y < MAP_ROWS / 2; y++) {
        for (int x = MAP_COLS / 2; x < MAP_COLS; x++) {
            if (grid[y][x] == EMPTY) {
                spawns[spawn_count] = {x, y};
                spawn_count++;
            }
        }
    }

    // terzo e quarto quadrante
    for (int y = MAP_ROWS / 2; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (grid[y][x] == EMPTY) {
                spawns[spawn_count] = {x, y};
                spawn_count++;
            }
        }
    }

    shuffle();
}

void Map::save_state() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            start_grid[y][x] = grid[y][x];
        }
    }
}

void Map::reset_state() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            grid[y][x] = start_grid[y][x];
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

void Map::shuffle() {
    for (int i = spawn_count - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Position tmp = spawns[i];
            spawns[i] = spawns[j];
            spawns[j] = tmp;
        }
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

CellContent Map::get_cell_content(Position p) {
    if (in_bounds(p)) {
        return grid[p.y][p.x];
    }
    else {
        return UNKNOWN;  // fallback
    }
}

void Map::set_cell_content(Position p, CellContent content) {
    if (in_bounds(p)) {
        grid[p.y][p.x] = content;
    }
}

void Map::clear_cell(Position p) {
    if (in_bounds(p)) {
        grid[p.y][p.x] = EMPTY;
    }
}

Position Map::get_random_spawn() {
    Position spawn = spawns[spawn_count - 1];
    spawn_count--;
    return spawn;
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