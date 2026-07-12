#include "Renderer.hpp"
#include "Bomb.hpp"
#include <cstdio>
#include <iostream>
#include <ncurses.h>
using namespace std;

void Renderer::init_colors() {
    if (has_colors()) {
        start_color();

        // Fallback per terminali senza 256 colori (es. pdcurses su Windows):
        // GREY (244) e ORANGE (208) non esistono e init_pair fallisce,
        // rendendo invisibili muri distruttibili e bombe
        short grey = GREY;
        short orange = ORANGE;
        if (COLORS < 256) {
            grey = COLOR_YELLOW;
            orange = COLOR_YELLOW;
        }

        init_pair(CP_SCREEN, COLOR_BLACK, COLOR_BLACK);
        init_pair(CP_UNBREAKABLE_WALL, COLOR_WHITE, COLOR_WHITE);
        init_pair(CP_BREAKABLE_WALL, grey, grey);
        init_pair(CP_DOOR, COLOR_GREEN, COLOR_BLACK);
        init_pair(CP_PLAYER, COLOR_CYAN, COLOR_BLACK);
        init_pair(CP_ENEMY, COLOR_RED, COLOR_BLACK);
        init_pair(CP_BOMB, orange, COLOR_BLACK);
        init_pair(CP_EXPLOSION, COLOR_YELLOW, COLOR_RED);
        init_pair(CP_BLINK, COLOR_WHITE, COLOR_BLACK);
        init_pair(CP_ITEM, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(CP_LIFE, COLOR_RED, COLOR_BLACK);
        init_pair(CP_TITLE, COLOR_WHITE, COLOR_BLACK);
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

// Titolo in stile "banner" da vecchio terminale: ogni lettera grande e'
// disegnata usando il suo stesso carattere come inchiostro.
const int TITLE_ROWS = 5;
const char* TITLE_BANNER[TITLE_ROWS] = {
    "BBBB   OOO  M   M BBBB  EEEEE RRRR  M   M  AAA  N   N",
    "B   B O   O MM MM B   B E     R   R MM MM A   A NN  N",
    "BBBB  O   O M M M BBBB  EEE   RRRR  M M M AAAAA N N N",
    "B   B O   O M   M B   B E     R  R  M   M A   A N  NN",
    "BBBB   OOO  M   M BBBB  EEEEE R   R M   M A   A N   N",
};

void Renderer::display_title() {
    int banner_w = 53;  // larghezza delle righe di TITLE_BANNER
    int top = map_start_p.y - 2 - 1 - TITLE_ROWS;  // sopra la riga HUD, con una riga vuota

    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    if (top >= 0 && max_x >= banner_w) {
        // Centrato rispetto alla griglia (che a sua volta e' centrata)
        int left = map_start_p.x + (GRID_COLS - banner_w) / 2;
        if (left < 0) {
            left = 0;
        }
        for (int i = 0; i < TITLE_ROWS; i++) {
            mvprintw(top + i, left, "%s", TITLE_BANNER[i]);
        }
    }
    else if (map_start_p.y - 3 >= 0) {
        // Terminale piccolo: titolo compatto
        const char* small_title = "B O M B E R M A N";
        int left = (max_x - 17) / 2;
        mvprintw(map_start_p.y - 3, left, "%s", small_title);
    }
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
}

void Renderer::display_lives(int lives) {
    move(map_start_p.y - 2, map_start_p.x);
    printw("VITE: ");

    // Disegno sempre MAX_LIVES slot: quelli persi diventano spazi,
    // cosi' i rombi vecchi vengono cancellati dal frame precedente
    for (int i = 0; i < MAX_LIVES; i++) {
        chtype glyph = ' ';
        if (i < lives) {
            glyph = ACS_DIAMOND | COLOR_PAIR(CP_LIFE) | A_BOLD;
        }
        addch(glyph);
        addch(' ');
    }
}

void Renderer::display_effect(double buff_remaining) {
    move(map_start_p.y - 1, map_start_p.x);

    char text[32] = "None";
    if (buff_remaining > 0.0) {
        // +1 = arrotondamento per eccesso: il conto parte da 10 e finisce a 1
        snprintf(text, sizeof(text), "Range %ds", (int) buff_remaining + 1);
    }
    // %-9s: padding a destra per cancellare il testo del frame precedente
    printw("EFFETTO: %-9s", text);
}

void Renderer::display_score(int score) {
    const int SCORE_MAX_LENGTH = 6;         // fino a 999999
    const int SCORE_LABEL_MAX_LENGTH = 13;  // length(SCORE: 999999) = 13
    int x = map_start_p.x + GRID_COLS - SCORE_LABEL_MAX_LENGTH;
    move(map_start_p.y - 1, x);
    printw("SCORE: %-*d", SCORE_MAX_LENGTH, score);
}

void Renderer::display_time(double time) {
    const int TIME_MAX_LENGTH = 4;         // length(1000) = 4
    // Stessa colonna di partenza di SCORE (l'etichetta piu' larga), cosi'
    // le due righe risultano incolonnate
    const int HUD_RIGHT_LABEL_LENGTH = 13;  // length(SCORE: 999999) = 13
    int x = map_start_p.x + GRID_COLS - HUD_RIGHT_LABEL_LENGTH;
    move(map_start_p.y - 2, x);
    printw("TIME: %-*d", TIME_MAX_LENGTH, (int) time);
}

void Renderer::display_colors_debug() {
    // Info di debug sotto la griglia
    if (map_start_p.y + GRID_ROWS < max_y) {
        move(map_start_p.y + GRID_ROWS, map_start_p.x);
        printw("COLORS: %d", COLORS);
    }
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

void Renderer::draw_items(Item* items) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].is_active()) {
            Position item_p = items[i].get_position();

            // chtype, non char: i simboli ACS_* sono definiti da curses
            // e non entrano in un singolo byte
            chtype glyph = 'R';  // ITEM_RANGE
            switch (items[i].get_type()) {
                case ITEM_LIFE:
                    glyph = ACS_DIAMOND;
                    break;

                case ITEM_TIME:
                    glyph = 'T';
                    break;

                case ITEM_SCORE:
                    glyph = ACS_STERLING;
                    break;

                default:  // ITEM_RANGE
                    break;
            }

            int y = item_p.y + map_start_p.y;
            int x = item_p.x + map_start_p.x;
            mvaddch(y, x, glyph | COLOR_PAIR(CP_ITEM));
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


void Renderer::render(Map& map, Player& player, int score, int time, double game_clock) {
    Level& level = map.get_current_level();
    Position player_p = player.get_position();

    display_title();
    display_lives(player.get_lives());
    display_effect(player.get_buff_remaining(game_clock));
    display_score(score);
    display_time(time);
    display_colors_debug();

    draw_grid(level.get_grid());
    draw_items(level.get_items());
    draw_bombs(level.get_bombs());
    draw_player(player_p);
    draw_dummy_enemies(level.get_dummy_enemies());
    draw_smart_enemies(level.get_smart_enemies());
    draw_explosions(level.get_grid());

    refresh();
}