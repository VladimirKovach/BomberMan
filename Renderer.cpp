#include "Renderer.hpp"
#include <ncurses.h>

void Renderer::init_colors() {
    if (has_colors()) {
        start_color();
    }
    init_pair(CP_SCREEN, COLOR_BLACK, COLOR_BLACK);
    init_pair(CP_UNBREAKABLE_WALL, COLOR_WHITE, COLOR_BLACK);
    init_pair(CP_PLAYER, COLOR_CYAN, COLOR_BLACK);
    init_pair(CP_ENEMY, COLOR_MAGENTA, COLOR_BLACK);
}

void Renderer::paint_it_black() {
    for (int y = 0; y < LINES; y++) {
        for (int x = 0; x < COLS; x++) {
            mvaddch(y, x, ' ' | COLOR_PAIR(CP_SCREEN));
        }
    }
}

Renderer::Renderer() {
    // Set map at the center of the screen
    map_start_p = {(COLS - MAP_COLS) / 2, (LINES - MAP_ROWS) / 2};

    init_colors();
    paint_it_black();
}


void Renderer::display_score(int score) {
    move(map_start_p.y - 2, map_start_p.x);
    printw("SCORE: %d", score);
}

void Renderer::display_time(int time) {
    const int TIME_MAX_LENGTH = 4;  // length(1000) = 4
    const int TIME_LABEL_MAX_LENGTH = 10;  // length(Time: 1000) = 10
    move(map_start_p.y - 2, map_start_p.x + MAP_COLS - TIME_LABEL_MAX_LENGTH);
    printw("TIME: %-*d", TIME_MAX_LENGTH, time);
}


char Renderer::get_cell_view(CellContent content) {
    char view = ' ';

    switch (content) {
        case BREAKABLE_WALL:
        case UNBREAKABLE_WALL:
            view = '#';
            break;

        case PLAYER:
            view = '@';
            break;

        case ENEMY:
            view = '$';
            break;

        default:  // EMPTY, UNKNOWN
            break;
    }

    return view;
}


ColorPair Renderer::get_cell_color(CellContent content) {
    ColorPair color = CP_SCREEN;

    switch (content) {
        case UNBREAKABLE_WALL:
            color = CP_UNBREAKABLE_WALL;
            break;

        case PLAYER:
            color = CP_PLAYER;
            break;

        case ENEMY:
            color = CP_ENEMY;
            break;

        default:  // EMPTY, UNKNOWN
            break;
    }

    return color;
}


void Renderer::draw_map(Map& map) {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            char cell_view = get_cell_view(map.get_cell_content({x, y}));
            ColorPair cell_color = get_cell_color(map.get_cell_content({x, y}));
            mvaddch(y + map_start_p.y, x + map_start_p.x, cell_view | COLOR_PAIR(cell_color));
        }
    }
}

void Renderer::draw_level(Map& map, int score, int time) {
    display_score(score);
    display_time(time);
    draw_map(map);
    refresh();
}