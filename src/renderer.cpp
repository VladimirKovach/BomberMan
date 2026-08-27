#include "renderer.hpp"
#include "bomb.hpp"
#include "item.hpp"
#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <ncurses.h>

void Renderer::init_colors() {
    start_color();

    //  ----- ADATTAMENTO DELLA GRAFICA PER WINDOWS -----

    // su pdcurses (Windows) use_default_colors() fallisce e ogni init_pair che
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

    //  -----------------------------------------------

    // COLOR_DEFAULT = -1 significa "usa lo sfondo nativo del terminale"
    init_pair(CP_SCREEN, bg, bg);
    init_pair(CP_WALL_SOLID, COLOR_WHITE, COLOR_WHITE);
    init_pair(CP_WALL_DESTRUCTIBLE, grey, grey);
    init_pair(CP_DOOR, COLOR_GREEN, bg);
    init_pair(CP_PLAYER, COLOR_CYAN, bg);
    init_pair(CP_ENEMY, COLOR_RED, bg);
    init_pair(CP_BOMB, COLOR_RED, bg);
    init_pair(CP_EXPLOSION, COLOR_RED, COLOR_RED);
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

Renderer::~Renderer() {
    delwin(map_window);
    delwin(info_window);
}



// Titolo in stile "banner" da vecchio terminale
const int TITLE_ROWS = 5;
const char* TITLE_BANNER[TITLE_ROWS] = {
    "****    ***   *   *  ****   ****  ****   *   *   ***   *   *",
    "*   *  *   *  ** **  *   *  *     *   *  ** **  *   *  **  *",
    "****   *   *  * * *  ****   ****  ****   * * *  *****  * * *",
    "*   *  *   *  *   *  *   *  *     *  *   *   *  *   *  *  **",
    "****    ***   *   *  ****   ****  *   *  *   *  *   *  *   *",
};

void Renderer::display_title() {
    const char* title = "B O M B E R M A N";

    // Lunghezza calcolata con un loop manuale (niente strlen)
    int len = 0;
    while (title[len] != '\0') len++;

    // Il titolo sta due righe sopra la mappa, cioe' sopra la riga EFFETTO.
    // Se il terminale e' troppo basso non c'e' spazio: non disegniamo nulla.
    int row = map_start_y - 3;
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

void Renderer::display_effect(int buff_remaining) {
    move(map_start_y - 1, map_start_x);

    char text[32] = "None";

    // Il padding a larghezza fissa serve a cancellare il testo del frame
    // precedente ("Range 10s" è la stringa piu lunga prevista: 9 caratteri).
    if (buff_remaining > 0) {
        int seconds = (buff_remaining + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND;
        printw("EFFETTO: Range %-3d", seconds);
    }
    else {
        printw("EFFETTO: %-9s", "None");
    }
}


void Renderer::draw_grid(Map& map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Cell c = map.get_cell({y, x});

            switch (c) {
                case WALL_SOLID:
                    mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_WALL_SOLID));
                    break;

                case WALL_DESTRUCTIBLE:
                    mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_WALL_DESTRUCTIBLE));
                    break;

                case DOOR_PREV:
                    mvwaddch(map_window, y, x, '<' | COLOR_PAIR(CP_DOOR));
                    break;

                case DOOR_NEXT:
                    mvwaddch(map_window, y, x, '>' | COLOR_PAIR(CP_DOOR));
                    break;

                default:
                    mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_SCREEN));
                    break;
            }
        }
    }
}

void Renderer::draw_bombs(Bomb* bombs) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            Position p = bombs[i].get_position();
            mvwaddch(map_window, p.y, p.x, 'O' | COLOR_PAIR(CP_BOMB));
        }
    }
}

void Renderer::draw_items(Item* items) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].is_active()) {
            Position p = items[i].get_position();

            // chtype (non char): i simboli ACS_* sono definiti da ncurses
            // e non entrano in un singolo byte
            chtype glyph = 'R';

            switch (items[i].get_type()) {
                case ITEM_LIFE:
                    glyph = ACS_DIAMOND;
                    break;

                case ITEM_SCORE:
                    glyph = ACS_STERLING;
                    break;

                case ITEM_TIME:
                    glyph = 'T';
                    break;

                default:  // ITEM_RANGE
                    break;
            }

            mvwaddch(map_window, p.y, p.x, glyph | COLOR_PAIR(CP_ITEM));
        }
    }
}

void Renderer::draw_player(Player& player) {
    Position p = player.get_position();
    mvwaddch(map_window, p.y, p.x, '@' | COLOR_PAIR(CP_PLAYER));
}

void Renderer::draw_enemies(Level& level) {
    Chaser* chasers = level.get_chasers();
    Roamer* roamers = level.get_roamers();
    Walker* walkers = level.get_walkers();

    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            Position p = chasers[i].get_position();
            mvwaddch(map_window, p.y, p.x, '!' | COLOR_PAIR(CP_ENEMY));
        }
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead()) {
            Position p = roamers[i].get_position();
            mvwaddch(map_window, p.y, p.x, '?' | COLOR_PAIR(CP_ENEMY));
        }
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead()) {
            Position p = walkers[i].get_position();
            mvwaddch(map_window, p.y, p.x, 'X' | COLOR_PAIR(CP_ENEMY));
        }
    }
}

void Renderer::draw_explosions(Map& map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map.is_explosion({y, x})) {
                mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_EXPLOSION));
            }
        }
    }
}

void Renderer::draw_map(Level& level, Player& player) {
    werase(map_window);

    draw_grid(level.get_map());
    draw_items(level.get_items());
    draw_bombs(level.get_bombs());
    draw_player(player);
    draw_enemies(level);
    draw_explosions(level.get_map());

    wnoutrefresh(map_window);
}

void Renderer::draw_info(Level& level, Player& player, int score, int time) {
    werase(info_window);
    box(info_window, 0, 0);

    mvwprintw(info_window, 1, 1, "LEVEL: %d", level.get_number());

    int lives = player.get_lives();
    mvwprintw(info_window, 3, 1, "LIVES:");
    for (int i = 0; i < lives; i++) {
        waddch(info_window, ' ');
        waddch(info_window, ACS_DIAMOND | COLOR_PAIR(CP_ITEM));
    }

    mvwprintw(info_window, 5, 1, "SCORE: %d", score);
    mvwprintw(info_window, 7, 1, "TIME: %d", time);

    wnoutrefresh(info_window);
}

void Renderer::render(LevelManager& level_manager, Player& player, int score, int time) {
    Level& level = level_manager.get_current_level();

    display_title();
    display_effect(player.get_buff_remaining());

    wnoutrefresh(stdscr); // <-- stdscr per primo: fa da sfondo

    draw_map(level, player);
    draw_info(level, player, score, time);

    doupdate();
}