#include "Map.hpp"
#include <cstdlib>
using namespace std;

bool equal(Position p, Position q) {
    return p.y == q.y && p.x == q.x;
}

void Map::get_spawns() {
    spawns_count = 0;

    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            Position p = {y, x};
            if (!safe_zone(p) && grid[y][x] == EMPTY) {
                spawns[spawns_count] = p;
                spawns_count++;
            }
        }
    }
}

// Fisher-Yates shuffle
void Map::shuffle_spawns() {
    for (int i = spawns_count - 1; i >= 0; i--) {
        int j = rand() % (i + 1);

        if (j != i) {
            Position p = spawns[i];
            spawns[i] = spawns[j];
            spawns[j] = p;
        }
    }
}

// primo quadrante
bool Map::safe_zone(Position p) {
    return p.y >= 0 && p.y < MAP_HEIGHT / 2 && p.x >= 0 && p.x < MAP_WIDTH / 2;
}

void Map::place_breakable_walls(int percentage) {
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            if (rand() % 100 < percentage) {
                grid[y][x] = BREAKABLE_WALL;
            }
        }
    }

    grid[1][1] = EMPTY;
    grid[1][2] = EMPTY;
    grid[2][1] = EMPTY;
}

bool Map::out_of_bounds(Position p) {
    return p.y < 0 || p.y >= MAP_HEIGHT || p.x < 0 || p.x >= MAP_WIDTH;
}

Map::Map(int difficulty) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = EMPTY;
            explosions[y][x] = false;
        }
    }

    // muri indistruttibili esterni
    for (int x = 0; x < MAP_WIDTH; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[MAP_HEIGHT - 1][x] = UNBREAKABLE_WALL;
    }
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][MAP_WIDTH - 1] = UNBREAKABLE_WALL;
    }

    // muri indistruttibili interni (pattern a scacchiera con buchi)
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            if (y % 2 == 0 && x % 2 == 0 && rand() % 2 == 0) {
                grid[y][x] = UNBREAKABLE_WALL;
            }
        }
    }

    place_breakable_walls(difficulty * 5);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            start_grid[y][x] = grid[y][x];
        }
    }

    get_spawns();
    shuffle_spawns();
}

void Map::reset() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = start_grid[y][x];
            explosions[y][x] = false;
        }
    }
}

Position Map::get_random_spawn() {
    Position spawn = spawns[spawns_count - 1];
    spawns_count--;
    return spawn;
}

Cell Map::get_cell(Position p) {
    if (out_of_bounds(p)) {
        return NONE;
    }

    return grid[p.y][p.x];
}

void Map::set_cell(Position p, Cell c) {
    if (!out_of_bounds(p)) {
        grid[p.y][p.x] = c;
    }
}

bool Map::is_wall(Position p) {
    if (out_of_bounds(p)) {
        return false;
    }

    Cell c = grid[p.y][p.x];
    return c == BREAKABLE_WALL || c == UNBREAKABLE_WALL;
}

bool Map::is_door(Position p) {
    if (out_of_bounds(p)) {
        return false;
    }

    Cell c = grid[p.y][p.x];
    return c == ENTRANCE || c == EXIT;
}

bool Map::is_explosion(Position p) {
    return !out_of_bounds(p) && explosions[p.y][p.x];
}

void Map::set_explosion(Position p) {
    if (!out_of_bounds(p)) {
        explosions[p.y][p.x] = true;
    }
}

void Map::unset_explosion(Position p) {
    if (!out_of_bounds(p)) {
        explosions[p.y][p.x] = false;
    }
}

void Map::open_entrance() {
    grid[1][0] = ENTRANCE;
}

void Map::close_entrance() {
    grid[1][0] = UNBREAKABLE_WALL;
}

void Map::open_exit() {
    grid[1][MAP_WIDTH - 1] = EXIT;
}

void Map::close_exit() {
    grid[1][MAP_WIDTH - 1] = UNBREAKABLE_WALL;
}