#ifndef MAP_HPP
#define MAP_HPP

enum CellContent {
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    PLAYER,
    BOMB,
    DUMMY_ENEMY,
    SMART_ENEMY,
    ITEM,
    EXPLOSION,
    DOOR_NEXT,  // porta per andare al livello successivo
    DOOR_PREV,  // porta per tornare al livello precedente
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

class Map {
protected:
    CellContent grid[MAP_ROWS][MAP_COLS];
    int explosion_timer[MAP_ROWS][MAP_COLS];
    Position start_p;
    Position empty_cells[MAX_EMPTY_CELLS];
    int empty_cells_count;

    void add_empty_cell(Position p);

    void remove_empty_cell(Position p);

    bool is_safe_zone(int x, int y);

    void place_unbreakable_walls();

    void place_breakable_walls(int difficulty);

public:
    Map(int difficulty = 1);

    bool cell_exists(Position p);

    bool is_empty_cell(Position p);

    bool is_walkable_cell(Position p);

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