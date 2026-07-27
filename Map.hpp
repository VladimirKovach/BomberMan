#ifndef MAP_HPP
#define MAP_HPP

enum Cell {
    NONE,
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    ENTRANCE,
    EXIT
};

struct Position {
    int y;
    int x;
};

bool equal(Position p, Position q);

const int MAP_HEIGHT = 21;
const int MAP_WIDTH = 41;
const int MAX_SPAWNS = MAP_HEIGHT * MAP_WIDTH;
const int MAX_DIFFICULTY = 5;

class Map {
protected:
    Cell grid[MAP_HEIGHT][MAP_WIDTH];
    Cell start_grid[MAP_HEIGHT][MAP_WIDTH];

    bool explosions[MAP_HEIGHT][MAP_WIDTH];

    void save_state();

    // Possibili posizioni in cui piazzare nemici
    Position spawns[MAX_SPAWNS];
    int spawn_count;

    void shuffle_spawns();

    bool in_bounds(Position p);

    bool is_safe_zone(Position p);
    void place_unbreakable_walls();
    void place_breakable_walls(int difficulty);  // da migliorare

public:
    Map(int difficulty = 1);

    void reset();

    Position get_random_spawn();

    // Gestione celle
    Cell get_cell(Position p);
    void set_cell(Position p, Cell c);
    bool is_walkable(Position p);
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
};

#endif