#ifndef MAP_HPP
#define MAP_HPP

enum CellContent {
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    PLAYER,
    BOMB,
    ENEMY,
    ITEM,
    UNKNOWN
};

struct Position {
    int x;
    int y;
};

bool positions_equal(Position p1, Position p2);

const int MAP_ROWS = 25;
const int MAP_COLS = MAP_ROWS * 2;
const int MAX_EMPTY_CELLS = MAP_ROWS * MAP_COLS;


class Map {
protected:
    CellContent grid[MAP_ROWS][MAP_COLS];
    Position start_p;

    Position empty_cells[MAX_EMPTY_CELLS];
    int empty_cells_count;

    void add_empty_cell(Position p);

    void remove_empty_cell(Position p);

public:
    Map();

    bool cell_exists(Position p);

    // Precondition: cell_exists(p)
    bool is_empty_cell(Position p);

    bool is_walkable_cell(Position p);

    Position get_random_empty_cell();

    CellContent get_cell_content(Position p);

    void set_cell_content(Position p, CellContent content);

    void clear_cell(Position p);

    void open_next_level_door();
};

#endif