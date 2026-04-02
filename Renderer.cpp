#include "Renderer.hpp"
#include "Bomb.hpp"
#include <curses.h>

void Renderer::init_colors() {
    if (has_colors()) {
        start_color();
    }
    init_pair(CP_SCREEN, COLOR_BLACK, COLOR_BLACK);

    init_pair(CP_UNBREAKABLE_WALL, COLOR_WHITE, COLOR_WHITE);
    init_pair(CP_BREAKABLE_WALL, 244, 244);  // 244 = grey

    init_pair(CP_PLAYER, COLOR_CYAN, COLOR_BLACK);
    init_pair(CP_ENEMY, COLOR_MAGENTA, COLOR_BLACK);

    init_pair(CP_BOMB, COLOR_RED, COLOR_BLACK);
    init_pair(CP_BOMB_BLINK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CP_EXPLOSION, COLOR_WHITE, COLOR_RED);
    init_pair(CP_EXPLOSION_FADE, COLOR_YELLOW, COLOR_RED);

    init_pair(CP_DOOR, COLOR_GREEN, COLOR_BLACK);
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
    const int TIME_LABEL_MAX_LENGTH = 10;  // length(TIME: 1000) = 10
    int px = map_start_p.x + MAP_COLS - TIME_LABEL_MAX_LENGTH;
    move(map_start_p.y - 2, px);
    printw("TIME: %-*d", TIME_MAX_LENGTH, time);
}

void Renderer::display_level_number(int level_number) {
    const int LEVEL_LABEL_MAX_LENGTH = 8;  // length(LEVEL: 5) = 8
    int px = map_start_p.x + ((MAP_COLS - LEVEL_LABEL_MAX_LENGTH) / 2);
    move(map_start_p.y - 4, px);
    printw("LEVEL: %d", level_number);
}


char Renderer::get_cell_view(CellContent content) {
    char view = ' ';

    switch (content) {
        case PLAYER:
            view = '@';
            break;

        case ENEMY:
            view = '$';
            break;

        case BOMB:
            view = 'O';
            break;

        case EXPLOSION:
            view = '*';
            break;

        case DOOR_NEXT:
            view = '>';
            break;

        case DOOR_PREV:
            view = '<';
            break;

        default:  // BREAKABLE_WALL, UNBREAKABLE_WALL, EMPTY, UNKNOWN
            break;
    }

    return view;
}


ColorPair Renderer::get_cell_color(CellContent content) {
    ColorPair color = CP_SCREEN;

    switch (content) {
        case BREAKABLE_WALL:
            color = CP_BREAKABLE_WALL;
            break;

        case UNBREAKABLE_WALL:
            color = CP_UNBREAKABLE_WALL;
            break;

        case PLAYER:
            color = CP_PLAYER;
            break;

        case ENEMY:
            color = CP_ENEMY;
            break;

        case BOMB:
            color = CP_BOMB;
            break;

        case EXPLOSION:
            color = CP_EXPLOSION;
            break;

        case DOOR_NEXT:
        case DOOR_PREV:
            color = CP_DOOR;
            break;

        default:  // EMPTY, UNKNOWN
            break;
    }

    return color;
}


void Renderer::draw_map(Map& map) {
    int frame_counter = 0;
    frame_counter++;

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            CellContent content = map.get_cell_content({x, y});
            char cell_view = get_cell_view(content);
            ColorPair cell_color = get_cell_color(content);

            // Lampeggio bomba
            if (content == BOMB) {
                if ((frame_counter / 5) % 2 == 0) {
                    cell_color = CP_BOMB;
                }
                else {
                    cell_color = CP_BOMB_BLINK;
                }
            }

            // Sfumatura esplosione
            if (content == EXPLOSION) {
                int explosion_timer = map.get_explosion_timer({x, y});
                if (explosion_timer < EXPLOSION_DURATION / 2) {
                    cell_color = CP_EXPLOSION_FADE;
                }
            }

            mvaddch(y + map_start_p.y, x + map_start_p.x, cell_view | COLOR_PAIR(cell_color));
        }
    }
}


void Renderer::draw_level(Map& map, int score, int time, int level_number) {
    display_score(score);
    display_time(time);
    display_level_number(level_number);
    draw_map(map);
    refresh();
}