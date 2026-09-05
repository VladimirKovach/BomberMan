#include "renderer.hpp"
#include "bomb.hpp"
#include "item.hpp"
#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "position.hpp"
#include <ncurses.h>

void Renderer::init_colors() {
    start_color();

    //  ----- ADATTAMENTO DELLA GRAFICA PER WINDOWS -----

    // su pdcurses (Windows) use_default_colors fallisce e ogni init_pair che
    // usa -1 viene ignorato, lasciando i caratteri bianco su nero
    short bg = COLOR_DEFAULT;
    if (use_default_colors() == ERR) {
        bg = COLOR_BLACK;
    }

    // fallback per terminali senza 256 colori, per esempio pdcurses (Windows)
    // GREY (244) non esiste, quindi init_pair fallisce, rendendo invisibili i
    // muri distruttibili.
    short grey = COLOR_GREY;
    if (COLORS < 256) {
        grey = COLOR_YELLOW;
    }

    //  -------------------------------------------------

    // COLOR_DEFAULT = -1 significa "usa lo sfondo nativo del terminale"
    init_pair(CP_SCREEN, bg, bg);
    init_pair(CP_WALL_SOLID, COLOR_WHITE, COLOR_WHITE);
    init_pair(CP_WALL_DESTRUCTIBLE, grey, grey);
    init_pair(CP_DOOR, COLOR_GREEN, bg);
    init_pair(CP_PLAYER, COLOR_CYAN, bg);
    init_pair(CP_ENEMY, COLOR_RED, bg);
    init_pair(CP_BOMB, COLOR_RED, bg);
    init_pair(CP_BOMB_BLINK, COLOR_WHITE, bg);
    init_pair(CP_EXPLOSION, COLOR_YELLOW, COLOR_RED);
    init_pair(CP_ITEM, COLOR_MAGENTA, bg);
    init_pair(CP_TITLE, COLOR_WHITE, bg);
}

Renderer::Renderer() {
    getmaxyx(stdscr, max_y, max_x);

    map_start_y = (max_y - MAP_HEIGHT) / 2;
    map_start_x = (max_x - MAP_WIDTH) / 2;

    map_window = newwin(MAP_HEIGHT, MAP_WIDTH, map_start_y, map_start_x);
    info_window = newwin(MAP_HEIGHT, INFO_WIDTH, map_start_y, map_start_x + MAP_WIDTH + 2);

    init_colors();
}

void Renderer::display_title() {
    const char* title = "B O M B E R M A N";

    // Lunghezza calcolata con un loop manuale (niente strlen)
    int len = 0;
    while (title[len] != '\0') len++;

    // Il titolo si trova due righe sopra la mappa
    // Se il terminale e' troppo basso non c'e' spazio: non disegniamo nulla.
    int row = map_start_y - 2;
    if (row < 0) {
        return;
    }

    int left = (max_x - len) / 2;
    if (left < 0) {
        left = 0;
    }

    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    mvprintw(row, left, "%s", title);
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
}

void Renderer::draw_cell(Level& level, Player& player, Position p) {
    Map& map = level.get_map();

    EnemyType enemy_type;
    ItemType item_type;
    bool blinking;

    // chtype (non char): i simboli ACS_* sono definiti da ncurses
    // e non entrano in un singolo byte
    chtype glyph;

    // Ordine di priorità: vince la prima condizione vera, quindi ciò che
    // sta "sopra" copre ciò che sta sotto.
    if (map.is_explosion(p)) {
        // Il | non è un "oppure", è una fusione di campi.
        glyph = '*' | COLOR_PAIR(CP_EXPLOSION);
    }
    else if (level.has_enemy(p, enemy_type)) {
        switch (enemy_type) {
            case ENEMY_CHASER:
                glyph = '!' | COLOR_PAIR(CP_ENEMY);
                break;

            case ENEMY_ROAMER:
                glyph = '?' | COLOR_PAIR(CP_ENEMY);
                break;

            default:  // ENEMY_WALKER
                glyph = 'X' | COLOR_PAIR(CP_ENEMY);
                break;
        }
    }
    else if (equal(player.get_position(), p)) {
        glyph = '@' | COLOR_PAIR(CP_PLAYER);
    }
    else if (level.has_bomb(p, blinking)) {
        if (blinking) {
            glyph = 'O' | COLOR_PAIR(CP_BOMB_BLINK);
        }
        else {
            glyph = 'O' | COLOR_PAIR(CP_BOMB);
        }
    }
    else if (level.has_item(p, item_type)) {
        switch (item_type) {
            case ITEM_LIFE:
                glyph = ACS_DIAMOND | COLOR_PAIR(CP_ITEM);
                break;

            case ITEM_SCORE:
                glyph = ACS_STERLING | COLOR_PAIR(CP_ITEM);
                break;

            case ITEM_TIME:
                glyph = 'T' | COLOR_PAIR(CP_ITEM);
                break;

            default:  // ITEM_RANGE
                glyph = 'R' | COLOR_PAIR(CP_ITEM);
                break;
        }
    }
    else {
        switch (map.get_cell(p)) {
            case WALL_SOLID:
                glyph = ' ' | COLOR_PAIR(CP_WALL_SOLID);
                break;

            case WALL_DESTRUCTIBLE:
                glyph = ' ' | COLOR_PAIR(CP_WALL_DESTRUCTIBLE);
                break;

            case DOOR_PREV:
                glyph = '<' | COLOR_PAIR(CP_DOOR);
                break;

            case DOOR_NEXT:
                glyph = '>' | COLOR_PAIR(CP_DOOR);
                break;

            default:
                glyph = ' ' | COLOR_PAIR(CP_SCREEN);
                break;
        }
    }

    mvwaddch(map_window, p.y, p.x, glyph);
}

void Renderer::display_lives(int lives) {
    mvwprintw(info_window, 3, 1, "LIVES:");

    for (int i = 0; i < lives; i++) {
        waddch(info_window, ' ');
        waddch(info_window, ACS_DIAMOND | COLOR_PAIR(CP_ITEM));
    }
}

void Renderer::display_effect(int buff_remaining) {
    if (buff_remaining > 0) {
        int seconds = (buff_remaining + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND;
        mvwprintw(info_window, 5, 1, "EFFECT: RANGE %d S", seconds);
    }
    else {
        mvwprintw(info_window, 5, 1, "EFFECT: NONE");
    }
}

void Renderer::draw_map(Level& level, Player& player) {
    werase(map_window);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            draw_cell(level, player, {y, x});
        }
    }

    wnoutrefresh(map_window);
}

void Renderer::draw_info(Level& level, Player& player, int score, int time) {
    werase(info_window);
    box(info_window, 0, 0);

    mvwprintw(info_window, 1, 1, "LEVEL: %d", level.get_number());

    display_lives(player.get_lives());
    display_effect(player.get_buff_remaining());

    mvwprintw(info_window, 7, 1, "SCORE: %d", score);
    mvwprintw(info_window, 9, 1, "TIME: %d", time);

    wnoutrefresh(info_window);
}

void Renderer::render(LevelManager& level_manager, Player& player, int score, int time) {
    Level& level = level_manager.get_current_level();

    display_title();

    // stdscr fa da sfondo: va messo nel virtual screen PRIMA delle sottofinestre,
    // altrimenti il titolo non arriva mai al terminale (doupdate legge solo ciò
    // che le wnoutrefresh hanno depositato) e getch() lo ridisegna per conto suo.
    wnoutrefresh(stdscr);

    draw_map(level, player);
    draw_info(level, player, score, time);

    doupdate();
}

void Renderer::free_windows() {
    delwin(map_window);
    delwin(info_window);
}