#ifndef GRID_HPP
#define GRID_HPP

enum Cell {
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    ENTRANCE,
    EXIT,
    UNKNOWN,
};

struct Position {
    int y;
    int x;
};

bool positions_equal(Position p, Position q);

const int GRID_ROWS = 21;
const int GRID_COLS = 41;
const int MAX_SPAWNS = GRID_ROWS * GRID_COLS;
const int MAX_DIFFICULTY = 5;

class Grid {
protected:
    Cell grid[GRID_ROWS][GRID_COLS];
    Cell start_grid[GRID_ROWS][GRID_COLS];

    bool explosions[GRID_ROWS][GRID_COLS];

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
    Grid(int difficulty = 1);

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
    void open_entrance();
    void close_entrance();
    void open_exit();
    void close_exit();
};

#endif