#ifndef MAP_HPP
#define MAP_HPP

enum CellContent {
    EMPTY,
    BREAKABLE_WALL,
    UNBREAKABLE_WALL,
    PLAYER,
    BOMB,
    ENEMY,
    SPECIAL_ENEMY,
    ITEM,
    UNKNOWN
};

struct Position {
    int x;
    int y;
};

const int MAP_ROWS = 30;
const int MAP_COLS = MAP_ROWS * 2;


class Map {
protected:
    CellContent grid[MAP_ROWS][MAP_COLS];
    Position start_pos;

public:
    Map();

    bool cell_exists(Position pos);

    bool is_empty_cell(Position pos);

    CellContent get_cell_content(Position pos);

    void set_cell_content(Position pos, CellContent content);

    void clear_cell(Position pos);
};

#endif