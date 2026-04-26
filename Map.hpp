#ifndef MAP_HPP
#define MAP_HPP

enum CellContent {
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    PLAYER,
    BOMB,
    EXPLOSION,
    DUMMY_ENEMY,
    SMART_ENEMY,
    NEXT_DOOR,
    PREV_DOOR,
    UNKNOWN
};

struct Position {
    int x;
    int y;
};

bool positions_equal(Position p1, Position p2);

const int MAP_ROWS = 20;
const int MAP_COLS = MAP_ROWS * 2;
const int MAX_EMPTY_CELLS = MAP_ROWS * MAP_COLS;

const int MAX_DIFFICULTY = 5;

class Map {
protected:
    CellContent grid[MAP_ROWS][MAP_COLS];

    int explosion_timer[MAP_ROWS][MAP_COLS];  // da togliere

    Position empty_cells[MAX_EMPTY_CELLS];
    int empty_cells_count;

    void add_empty_cell(Position p);

    void remove_empty_cell(Position p);

    bool is_safe_zone(int x, int y);  // nome da cambiare

    void place_unbreakable_walls();

    void place_breakable_walls(int difficulty);

public:
    Map(int difficulty = 1);

    bool in_bounds(Position p);

    bool is_empty_cell(Position p);

    bool is_wall(Position p);

    bool is_door(Position p);

    bool is_enemy(Position p);

    bool is_explosion(Position p);

    Position get_random_empty_cell();

    CellContent get_cell_content(Position p);

    void set_cell_content(Position p, CellContent content);

    void clear_cell(Position p);

    // Porte tra livelli
    void open_next_door();

    void open_prev_door();

    void close_next_door();

    void close_prev_door();

    // da togliere
    void set_explosion(Position p, int duration);
    int get_explosion_timer(Position p);
    void update_explosions();
};

#endif