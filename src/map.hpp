#ifndef MAP_HPP
#define MAP_HPP

#include "utils.hpp"

enum Cell {
    NONE,
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    ENTRANCE,
    EXIT,
};

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 41;
const int MAX_SPAWNS = MAP_HEIGHT * MAP_WIDTH;

class Map {
protected:
    Cell grid[MAP_HEIGHT][MAP_WIDTH];
    Cell start_grid[MAP_HEIGHT][MAP_WIDTH];

    bool explosion[MAP_HEIGHT][MAP_WIDTH];

    // Possibili posizioni in cui spawnare nemici
    Position spawns[MAX_SPAWNS];
    int spawns_count;

    void get_spawns();
    void shuffle_spawns();
    bool safe_zone(Position p);

    void place_breakable_walls(int percentage);

    bool out_of_bounds(Position p);

public:
    Map(int difficulty = 1);

    Position get_random_spawn();

    // Gestione celle
    Cell get_cell(Position p);
    void set_cell(Position p, Cell c);

    bool is_wall(Position p);
    bool is_door(Position p);

    // Gestione esplosioni
    bool is_explosion(Position p);
    void set_explosion(Position p);
    void unset_explosion(Position p);

    // Porte tra livelli
    // ENTRANCE: bordo superiore sinistro
    // EXIT: bordo superiore destro
    void open_entrance();
    void close_entrance();
    void open_exit();
    void close_exit();

    void reset();
};

#endif