#include "Map.hpp"
#include <cstdlib>

bool positions_equal(Position p1, Position p2) {
    return p1.x == p2.x && p1.y == p2.y;
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

CellContent Map::get_cell_content(Position p) {
    if (cell_exists(p)) {
        return grid[p.y][p.x];
    }
    else {
        return UNKNOWN;
    }
}

void Map::set_cell_content(Position p, CellContent content) {
    if (cell_exists(p) && is_empty_cell(p)) {
        grid[p.y][p.x] = content;
        remove_empty_cell(p);
    }
}

void Map::clear_cell(Position p) {
    if (cell_exists(p) && !is_empty_cell(p)) {
        grid[p.y][p.x] = EMPTY;
        add_empty_cell(p);
    }
}

void Map::open_next_level_door() {
    clear_cell({MAP_COLS - 1, 1});
}