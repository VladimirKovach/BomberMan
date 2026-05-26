#include "Renderer.hpp"
#include "Bomb.hpp"
#include <iostream>
#include <ncurses.h>
using namespace std;

void Renderer::init_colors() {
    if (has_colors()) {
        start_color();
        init_pair(CP_SCREEN, COLOR_BLACK, COLOR_BLACK);
        init_pair(CP_UNBREAKABLE_WALL, COLOR_WHITE, COLOR_WHITE);
        init_pair(CP_BREAKABLE_WALL, GREY, GREY);
        init_pair(CP_DOOR, COLOR_GREEN, COLOR_BLACK);
        init_pair(CP_PLAYER, COLOR_CYAN, COLOR_BLACK);
        init_pair(CP_ENEMY, COLOR_RED, COLOR_BLACK);
        init_pair(CP_BOMB, ORANGE, COLOR_BLACK);
        init_pair(CP_EXPLOSION, COLOR_YELLOW, COLOR_RED);
        init_pair(CP_BLINK, COLOR_WHITE, COLOR_BLACK);
    }
    else {
        endwin();
        cout << "Error: terminal does not support colors\n";
        exit(1);
    }
}

void Renderer::paint_it_black() {
    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            mvaddch(y, x, ' ' | COLOR_PAIR(CP_SCREEN));
        }
    }
}

Renderer::Renderer() {
    getmaxyx(stdscr, max_y, max_x);
    if (max_y < GRID_ROWS || max_x < GRID_COLS) {
        endwin();
        cout << "Error: terminal size is not sufficient\n";
        exit(1);
    }

    // Mappa al centro dello schermo
    map_start_p = {(max_y - GRID_ROWS) / 2, (max_x - GRID_COLS) / 2};
    init_colors();
    paint_it_black();
}

void Renderer::display_score(int score) {
    move(map_start_p.y - 2, map_start_p.x);
    printw("SCORE: %d", score);
}

void Renderer::display_time(double time) {
    const int TIME_MAX_LENGTH = 4;         // length(1000) = 4
    const int TIME_LABEL_MAX_LENGTH = 10;  // length(TIME: 1000) = 10
    int x = map_start_p.x + GRID_COLS - TIME_LABEL_MAX_LENGTH;
    move(map_start_p.y - 2, x);
    printw("TIME: %-*d", TIME_MAX_LENGTH, (int) time);
}


char Renderer::get_cell_view(Cell c) {
    char view = ' ';

    switch (c) {
        case ENTRANCE:
            view = '<';
            break;

        case EXIT:
            view = '>';
            break;

        default:  // BREAKABLE_WALL, UNBREAKABLE_WALL, EMPTY, UNKNOWN
            break;
    }

    return view;
}


ColorPair Renderer::get_cell_color(Cell c) {
    ColorPair color = CP_SCREEN;

    switch (c) {
        case BREAKABLE_WALL:
            color = CP_BREAKABLE_WALL;
            break;

        case UNBREAKABLE_WALL:
            color = CP_UNBREAKABLE_WALL;
            break;

        case ENTRANCE:
        case EXIT:
            color = CP_DOOR;
            break;

        default:  // EMPTY, UNKNOWN
            break;
    }

    return color;
}


void Renderer::draw_grid(Grid& grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            Cell c = grid.get_cell({y, x});
            char cell_view = get_cell_view(c);
            ColorPair cell_color = get_cell_color(c);

            int ny = y + map_start_p.y;
            int nx = x + map_start_p.x;
            mvaddch(ny, nx, cell_view | COLOR_PAIR(cell_color));
        }
    }
}


void Renderer::draw_bombs(Bomb* bombs) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            Position bomb_p = bombs[i].get_position();
            ColorPair bomb_color = CP_BOMB;
            if (bombs[i].is_blinking()) {
                bomb_color = CP_BLINK;
            }
            int y = bomb_p.y + map_start_p.y;
            int x = bomb_p.x + map_start_p.x;
            mvaddch(y, x, 'O' | COLOR_PAIR(bomb_color));
        }
    }
}

void Renderer::draw_player(Position player_p) {
    int y = player_p.y + map_start_p.y;
    int x = player_p.x + map_start_p.x;
    mvaddch(y, x, '@' | COLOR_PAIR(CP_PLAYER));
}

void Renderer::draw_dummy_enemies(DummyEnemy* dummy_enemies) {
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_p = dummy_enemies[i].get_position();
            int y = dummy_p.y + map_start_p.y;
            int x = dummy_p.x + map_start_p.x;
            mvaddch(y, x, '?' | COLOR_PAIR(CP_ENEMY));
        }
    }
}

void Renderer::draw_smart_enemies(SmartEnemy* smart_enemies) {
    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_p = smart_enemies[i].get_position();
            int y = smart_p.y + map_start_p.y;
            int x = smart_p.x + map_start_p.x;
            mvaddch(y, x, '!' | COLOR_PAIR(CP_ENEMY));
        }
    }
}

void Renderer::draw_explosions(Grid& grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            if (grid.is_explosion({y, x})) {
                int ny = y + map_start_p.y;
                int nx = x + map_start_p.x;
                mvaddch(ny, nx, '*' | COLOR_PAIR(CP_EXPLOSION));
            }
        }
    }
}


void Renderer::render(Map& map, Position player_p, int score, int time) {
    Level& level = map.get_current_level();

    display_score(score);
    display_time(time);

    draw_grid(level.get_grid());
    draw_bombs(level.get_bombs());
    draw_player(player_p);
    draw_dummy_enemies(level.get_dummy_enemies());
    draw_smart_enemies(level.get_smart_enemies());
    draw_explosions(level.get_grid());

    refresh();
}