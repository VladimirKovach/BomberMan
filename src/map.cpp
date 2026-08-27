#include "map.hpp"
#include "utils.hpp"
#include <cstdlib>

void Map::save_spawns() {
    spawn_count = 0;

    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            Position spawn = {y, x};

            if (!safe_zone(spawn) && grid[y][x] == EMPTY) {
                spawns[spawn_count] = spawn;
                spawn_count++;
            }
        }
    }
}

// Fisher-Yates shuffle
void Map::shuffle_spawns() {
    for (int i = spawn_count - 1; i >= 0; i--) {
        int j = rand() % (i + 1);

        if (j != i) {
            Position p = spawns[i];
            spawns[i] = spawns[j];
            spawns[j] = p;
        }
    }
}

// SAFE ZONE - zona in cui non si possono spawnare i nemici
bool Map::safe_zone(Position p) {
    return p.y >= 0 && p.y < SAFE_ZONE_SIZE && p.x >= 0 && p.x < SAFE_ZONE_SIZE;
}

void Map::place_solid_walls() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        grid[y][0] = WALL_SOLID;              // bordo sinistro
        grid[y][MAP_WIDTH - 1] = WALL_SOLID;  // bordo destro
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
        grid[0][x] = WALL_SOLID;               // bordo superiore
        grid[MAP_HEIGHT - 1][x] = WALL_SOLID;  // bordo inferiore
    }

    // muri solidi interni (pattern a scacchiera con buchi)
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            if (y % 2 == 0 && x % 2 == 0 && rand() % 100 < 50) {
                grid[y][x] = WALL_SOLID;
            }
        }
    }
}

void Map::place_destructible_walls(int percentage) {
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            if (grid[y][x] == EMPTY && rand() % 100 < percentage) {
                grid[y][x] = WALL_DESTRUCTIBLE;
            }
        }
    }

    // Cella iniziale del giocatore, celle adiacenti e almeno una via di fuga:
    // con raggio 1 l'esplosione da (1,1) copre (1,2) e (2,1), quindi serve
    // una casella libera fuori dalla croce per poter bombardare senza morire.
    grid[1][1] = EMPTY;
    grid[1][2] = EMPTY;
    grid[2][1] = EMPTY;
    grid[1][3] = EMPTY;
}

void Map::save_start_grid() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            start_grid[y][x] = grid[y][x];
        }
    }
}

Map::Map(int difficulty) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = EMPTY;
            explosion[y][x] = 0;
        }
    }

    place_solid_walls();
    place_destructible_walls(BASE_WALL_PERCENTAGE + difficulty * 5);
    save_start_grid();

    save_spawns();
    shuffle_spawns();
}

Position Map::get_random_spawn() {
    Position spawn = spawns[spawn_count - 1];
    spawn_count--;
    return spawn;
}

Cell Map::get_cell(Position p) {
    if (out_of_bounds(p)) {
        return NONE;
    }

    return grid[p.y][p.x];
}

bool Map::out_of_bounds(Position p) {
    return p.y < 0 || p.y >= MAP_HEIGHT || p.x < 0 || p.x >= MAP_WIDTH;
}

bool Map::is_wall(Position p) {
    if (out_of_bounds(p)) {
        return false;
    }

    return grid[p.y][p.x] == WALL_SOLID || grid[p.y][p.x] == WALL_DESTRUCTIBLE;
}

bool Map::is_wall_solid(Position p){
    return !out_of_bounds(p) && grid[p.y][p.x] == WALL_SOLID;
}

bool Map::is_wall_destructible(Position p) {
    return !out_of_bounds(p) && grid[p.y][p.x] == WALL_DESTRUCTIBLE;
}

void Map::break_wall(Position p) {
    if (is_wall_destructible(p)) {
        grid[p.y][p.x] = EMPTY;
    }
}

bool Map::is_door(Position p) {
    if (out_of_bounds(p)) {
        return false;
    }

    return grid[p.y][p.x] == DOOR_PREV || grid[p.y][p.x] == DOOR_NEXT;
}

bool Map::is_door_prev(Position p) {
    return !out_of_bounds(p) && grid[p.y][p.x] == DOOR_PREV;
}

bool Map::is_door_next(Position p) {
    return !out_of_bounds(p) && grid[p.y][p.x] == DOOR_NEXT;
}

void Map::open_door_prev() {
    grid[1][0] = DOOR_PREV;
}

void Map::close_door_prev() {
    grid[1][0] = WALL_SOLID;
}

void Map::open_door_next() {
    grid[1][MAP_WIDTH - 1] = DOOR_NEXT;
}

void Map::close_door_next() {
    grid[1][MAP_WIDTH - 1] = WALL_SOLID;
}

bool Map::is_bomb(Position p) {
    return !out_of_bounds(p) && grid[p.y][p.x] == BOMB;
}

void Map::set_bomb(Position p) {
    if (!out_of_bounds(p)) {
        grid[p.y][p.x] = BOMB;
    }
}

void Map::unset_bomb(Position p) {
    if (is_bomb(p)) {
        grid[p.y][p.x] = EMPTY;
    }
}

bool Map::is_explosion(Position p) {
    return !out_of_bounds(p) && explosion[p.y][p.x] > 0;
}

void Map::set_explosion(Position p) {
    if (!out_of_bounds(p)) {
        explosion[p.y][p.x]++;
    }
}

void Map::unset_explosion(Position p) {
    if (!out_of_bounds(p)) {
        explosion[p.y][p.x]--;
    }
}

void Map::reset() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = start_grid[y][x];
            explosion[y][x] = false;
        }
    }
}