#include "Map.hpp"
#include <cstdlib>

bool positions_equal(Position p1, Position p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}


Map::Map(int difficulty) {
    // PASSO 1: Inizializza tutta la griglia come EMPTY
    empty_cells_count = 0;
    total_enemies = 0;
    alive_enemies = 0;

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            grid[y][x] = EMPTY;
        }
    }

    // PASSO 2: Bordi della mappa (muri indistruttibili)
    for (int x = 0; x < MAP_COLS; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[MAP_ROWS - 1][x] = UNBREAKABLE_WALL;
    }
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][MAP_COLS - 1] = UNBREAKABLE_WALL;
    }

    // PASSO 3: Muri indistruttibili interni (pattern a scacchiera)
    place_unbreakable_walls();

    // PASSO 4: Muri distruttibili (quantita' basata sulla difficolta')
    place_breakable_walls(difficulty);

    // PASSO 5: Riempi l'array empty_cells con tutte le celle rimaste EMPTY
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY) {
                add_empty_cell({x, y});
            }
        }
    }

    // PASSO 6: Inizializza la tabella di timer di esplosione a 0
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            explosion_timer[y][x] = 0;
        }
    }
}


// =====================================================
// is_safe_zone: controlla se la cella (x,y) NON deve
// mai ricevere muri distruttibili.
//
// Protegge solo la zona spawn del giocatore: (1,1), (2,1), (1,2)
// Senza questo, il giocatore potrebbe iniziare intrappolato.
// =====================================================
bool Map::is_safe_zone(int x, int y) {
    if (x == 1 && y == 1) return true;
    if (x == 2 && y == 1) return true;
    if (x == 1 && y == 2) return true;

    return false;
}


void Map::place_unbreakable_walls() {
    for (int y = 2; y < MAP_ROWS - 1; y += 2) {
        for (int x = 2; x < MAP_COLS - 1; x += 2) {
            grid[y][x] = UNBREAKABLE_WALL;
        }
    }
}


void Map::place_breakable_walls(int difficulty) {
    if (difficulty < 1) difficulty = 1;
    if (difficulty > 5) difficulty = 5;

    int percentage[6] = {0, 30, 37, 45, 52, 60};

    int total_empty = 0;
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            if (grid[y][x] == EMPTY && !is_safe_zone(x, y)) {
                total_empty++;
            }
        }
    }

    int walls_to_place = total_empty * percentage[difficulty] / 100;

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


bool Map::cell_exists(Position p) {
    return (
        p.x >= 0 && p.x < MAP_COLS &&
        p.y >= 0 && p.y < MAP_ROWS
    );
}

bool Map::is_empty_cell(Position p) {
    return grid[p.y][p.x] == EMPTY;
}

bool Map::is_walkable_cell(Position p) {
    return (
        cell_exists(p) &&
        grid[p.y][p.x] != BREAKABLE_WALL &&
        grid[p.y][p.x] != UNBREAKABLE_WALL
    );
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

            for (; i < empty_cells_count - 1; i++) {
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
        return {-1, -1};  // da gestire
    }
}

CellContent Map::get_cell_content(Position p) {
    if (cell_exists(p)) {
        return grid[p.y][p.x];
    }
    else {
        return UNKNOWN;
    }
}

void Map::set_cell_content(Position p, CellContent content) {
    if (cell_exists(p)) {
        if (is_empty_cell(p)) {
            remove_empty_cell(p);
        }
        grid[p.y][p.x] = content;
    }
}

void Map::clear_cell(Position p) {
    if (cell_exists(p) && !is_empty_cell(p)) {
        grid[p.y][p.x] = EMPTY;
        add_empty_cell(p);
    }
}


// =====================================================
// Porte tra livelli
//
// DOOR_NEXT (uscita): bordo inferiore destro
//   Posizione: ultima colonna (MAP_COLS-1), penultima riga (MAP_ROWS-2)
//
// DOOR_PREV (entrata): bordo superiore sinistro
//   Posizione: prima colonna (0), seconda riga (1)
// =====================================================
void Map::open_next_door() {
    Position door_pos = {MAP_COLS - 1, MAP_ROWS - 2};
    grid[door_pos.y][door_pos.x] = DOOR_NEXT;
}

void Map::open_prev_door() {
    Position door_pos = {0, 1};
    grid[door_pos.y][door_pos.x] = DOOR_PREV;
}

void Map::close_next_door() {
    Position door_pos = {MAP_COLS - 1, MAP_ROWS - 2};
    grid[door_pos.y][door_pos.x] = UNBREAKABLE_WALL;
}

void Map::close_prev_door() {
    Position door_pos = {0, 1};
    grid[door_pos.y][door_pos.x] = UNBREAKABLE_WALL;
}


// =====================================================
// Gestione nemici nel livello
// =====================================================
void Map::set_enemy_count(int count) {
    total_enemies = count;
    alive_enemies = count;
}

void Map::enemy_killed() {
    if (alive_enemies > 0) {
        alive_enemies--;
    }
}

int Map::get_alive_enemies() {
    return alive_enemies;
}

bool Map::all_enemies_dead() {
    return alive_enemies <= 0;
}


// =====================================================
// Esplosioni
// =====================================================
void Map::set_explosion(Position p, int duration) {
    if (cell_exists(p)) {
        grid[p.y][p.x] = EXPLOSION;
        explosion_timer[p.y][p.x] = duration;
    }
}

int Map::get_explosion_timer(Position p) {
    if (cell_exists(p)) {
        return explosion_timer[p.y][p.x];
    }
    return 0;
}

void Map::update_explosions() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (grid[y][x] == EXPLOSION) {
                explosion_timer[y][x]--;
                if (explosion_timer[y][x] <= 0) {
                    grid[y][x] = EMPTY;
                    explosion_timer[y][x] = 0;
                    add_empty_cell({x, y});
                }
            }
        }
    }
}