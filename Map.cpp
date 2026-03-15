#include "Map.hpp"

Map::Map() {
    // upper and lower bounds
    for (int x = 0; x < MAP_COLS; x++) {
        grid[0][x] = UNBREAKABLE_WALL;
        grid[MAP_ROWS - 1][x] = UNBREAKABLE_WALL;
    }

    // left and right bounds
    for (int y = 1; y < MAP_ROWS - 1; y++) {
        grid[y][0] = UNBREAKABLE_WALL;
        grid[y][MAP_COLS - 1] = UNBREAKABLE_WALL;
    }

    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            grid[y][x] = EMPTY;
        }
    }
}


bool Map::cell_exists(Position pos) {
    return (pos.x >= 0 && pos.x < MAP_COLS && pos.y >= 0 && pos.y < MAP_ROWS);
}

bool Map::is_empty_cell(Position pos) {
    return (cell_exists(pos) && grid[pos.y][pos.x] == EMPTY);
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
    if (is_empty_cell(pos)) {
        grid[pos.y][pos.x] = content;
    }
}

void Map::clear_cell(Position pos) {
    if (cell_exists(pos)) {
        grid[pos.y][pos.x] = EMPTY;
    }
}