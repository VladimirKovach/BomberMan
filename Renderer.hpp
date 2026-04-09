#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Map.hpp"

enum ColorPair {
    CP_NONE,  // terminal default color
    CP_SCREEN,
    CP_UNBREAKABLE_WALL,
    CP_BREAKABLE_WALL,
    CP_PLAYER,
    CP_ENEMY,
    CP_BOMB,
    CP_EXPLOSION,
    CP_DOOR
};

class Renderer {
protected:
    Position map_start_p;

    void init_colors();

    void paint_it_black();

    void display_score(int score);
    void display_time(double time);
    void display_level_number(int level_number);

    char get_cell_view(CellContent content);
    ColorPair get_cell_color(CellContent content);

    void draw_map(Map& map);

public:
    Renderer();

    void draw_level(Map& map, int score, int time, int level_number);
};

#endif