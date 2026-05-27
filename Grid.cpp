#include "Grid.hpp"
#include <algorithm>
#include <cstdlib>
using namespace std;

bool positions_equal(Position p, Position q) {
    return p.y == q.y && p.x == q.x;
}


void Grid::save_state() {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            start_grid[y][x] = grid[y][x];
        }
    }
}

// Fisher-Yates Shuffle
void Grid::shuffle_spawns() {
    for (int i = spawn_count - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Position tmp = spawns[i];
            spawns[i] = spawns[j];
            spawns[j] = tmp;
        }
    }
}

bool Grid::in_bounds(Position p) {
    return (p.y >= 0 && p.y < GRID_ROWS) && (p.x >= 0 && p.x < GRID_COLS);
}

// Controlla se la cella (y, x) non deve mai ricevere muri distruttibili
// Protegge solo la zona spawn del giocatore: (1, 1), (1, 2), (2, 1)
// Senza questo, il giocatore potrebbe iniziare intrappolato
bool Grid::is_safe_zone(Position p) {
    if (p.y == 1 && p.x == 1) {
        return true;
    }
    else if (p.y == 1 && p.x == 2) {
        return true;
    }
    else if (p.y == 2 && p.x == 1) {
        return true;
    }
    else {
        return false;
    }
}

void Grid::place_unbreakable_walls() {
    for (int y = 2; y < GRID_ROWS - 1; y += 2) {
        for (int x = 2; x < GRID_COLS - 1; x += 2) {
            int place = rand() % 2;
            if (place == 0) {  // successo
                grid[y][x] = UNBREAKABLE_WALL;
            }
        }
    }
}


void Grid::place_breakable_walls(int difficulty) {
    difficulty = max(difficulty, 1);
    difficulty = min(difficulty, MAX_DIFFICULTY);

    int percentage[MAX_DIFFICULTY] = {5, 10, 15, 20, 25};

    int empty_cells = 0;
    for (int y = 1; y < GRID_ROWS - 1; y++) {
        for (int x = 1; x < GRID_COLS - 1; x++) {
            if (grid[y][x] == EMPTY && !is_safe_zone({y, x})) {
                empty_cells++;
            }
        }
    }

    int walls_to_place = empty_cells * percentage[difficulty - 1] / 100;

    int placed = 0;
    int max_attempts = walls_to_place * 10;
    int attempts = 0;

    while (placed < walls_to_place && attempts < max_attempts) {
        int x = 1 + rand() % (GRID_COLS - 2);
        int y = 1 + rand() % (GRID_ROWS - 2);

        if (grid[y][x] == EMPTY && !is_safe_zone({y, x})) {
            grid[y][x] = BREAKABLE_WALL;
            placed++;
        }
        attempts++;
    }
}


Grid::Grid(int difficulty) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            explosions[y][x] = false;
        }
    }

    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            grid[y][x] = EMPTY;
        }
    }

    // Bordi della griglia (muri indistruttibili)
    for (int x = 0; x < GRID_COLS; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[GRID_ROWS - 1][x] = UNBREAKABLE_WALL;
    }
    for (int y = 1; y < GRID_ROWS - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][GRID_COLS - 1] = UNBREAKABLE_WALL;
    }

    // Muri indistruttibili interni (pattern a scacchiera, con buchi)
    place_unbreakable_walls();

    // Muri distruttibili (quantita' basata sulla difficolta')
    place_breakable_walls(difficulty);

    spawn_count = 0;

    // Secondo quadrante (primo quadrante escluso)
    for (int y = 0; y < GRID_ROWS / 2; y++) {
        for (int x = GRID_COLS / 2; x < GRID_COLS; x++) {
            if (grid[y][x] == EMPTY) {
                spawns[spawn_count] = {y, x};
                spawn_count++;
            }
        }
    }

    // Terzo e quarto quadrante
    for (int y = GRID_ROWS / 2; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (grid[y][x] == EMPTY) {
                spawns[spawn_count] = {y, x};
                spawn_count++;
            }
        }
    }

    shuffle_spawns();

    save_state();
}


void Grid::reset() {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            grid[y][x] = start_grid[y][x];
            explosions[y][x] = false;
        }
    }
}

Position Grid::get_random_spawn() {
    Position spawn = spawns[spawn_count - 1];
    spawn_count--;
    return spawn;
}

Cell Grid::get_cell(Position p) {
    if (in_bounds(p)) {
        return grid[p.y][p.x];
    }
    else {
        return UNKNOWN;  // fallback
    }
}

void Grid::set_cell(Position p, Cell c) {
    if (in_bounds(p)) {
        grid[p.y][p.x] = c;
    }
}

bool Grid::is_walkable(Position p) {
    if (in_bounds(p)) {
        Cell c = grid[p.y][p.x];
        return c != BREAKABLE_WALL && c != UNBREAKABLE_WALL;
    }
    else {
        return false;
    }
}

bool Grid::is_door(Position p) {
    if (in_bounds(p)) {
        return grid[p.y][p.x] == ENTRANCE || grid[p.y][p.x] == EXIT;
    }
    else {
        return false;
    }
}

bool Grid::is_explosion(Position p) {
    return in_bounds(p) && explosions[p.y][p.x];
}

void Grid::set_explosion(Position p) {
    if (in_bounds(p)) {
        explosions[p.y][p.x] = true;
    }
}

void Grid::unset_explosion(Position p) {
    if (in_bounds(p)) {
        explosions[p.y][p.x] = false;
    }
}

// ENTRANCE: bordo superiore sinistro
// EXIT: bordo superiore destro
void Grid::open_entrance() {
    grid[1][0] = ENTRANCE;
}

void Grid::close_entrance() {
    grid[1][0] = UNBREAKABLE_WALL;
}

void Grid::open_exit() {
    grid[1][GRID_COLS - 1] = EXIT;
}

void Grid::close_exit() {
    grid[1][GRID_COLS - 1] = UNBREAKABLE_WALL;
}