#include "Map.hpp"
#include <cstdlib>

bool positions_equal(Position p, Position q) {
    return (p.x == q.x && p.y == q.y);
}


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

    empty_cells_count = 0;

    for (int y = 1; y < MAP_ROWS - 1; y++) {
        for (int x = 1; x < MAP_COLS - 1; x++) {
            grid[y][x] = EMPTY;
            add_empty_cell({x, y});
        }
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