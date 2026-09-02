#ifndef MAP_HPP
#define MAP_HPP

#include "position.hpp"

enum Cell {
    NONE,
    EMPTY,
    WALL_SOLID,
    WALL_DESTRUCTIBLE,
    DOOR_PREV,
    DOOR_NEXT,
    BOMB,
};

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 41;
const int MAX_SPAWNS = MAP_HEIGHT * MAP_WIDTH;

// Lato del quadrato in cui non si possono spawnare nemici
const int SAFE_ZONE_SIZE = 5;

// Percentuale base di muri distruttibili; a questa si somma 5 per livello.
const int BASE_WALL_PERCENTAGE = 5;

class Map {
protected:
    Cell grid[MAP_HEIGHT][MAP_WIDTH];
    Cell start_grid[MAP_HEIGHT][MAP_WIDTH];

    // Numero di esplosioni presenti in ogni cella (sovrapposte se piu' di una)
    int explosion[MAP_HEIGHT][MAP_WIDTH];

    // Possibili posizioni in cui spawnare nemici
    Position spawns[MAX_SPAWNS];
    int spawn_count;

    void save_spawns();
    void shuffle_spawns();

    bool safe_zone(Position p);

    void place_solid_walls();
    void place_destructible_walls(int percentage);

    void save_start_grid();

public:
    Map(int difficulty = 1);

    // Restituisce uno spawn valido e lo rimuove dalla lista dei possibili spawn
    Position get_random_spawn();

    Cell get_cell(Position p);

    bool out_of_bounds(Position p);

    bool is_wall(Position p);
    bool is_wall_solid(Position p);
    bool is_wall_destructible(Position p);
    void break_wall(Position p);

    bool is_door(Position p);
    bool is_door_prev(Position p);
    bool is_door_next(Position p);

    void open_door_prev();
    void close_door_prev();
    void open_door_next();
    void close_door_next();

    bool is_bomb(Position p);
    void set_bomb(Position p);
    void unset_bomb(Position p);

    bool is_explosion(Position p);
    void set_explosion(Position p);
    void unset_explosion(Position p);

    void reset();
};

#endif