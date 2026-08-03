#include "renderer.hpp"
#include "level_manager.hpp"
#include "player.hpp"
#include "utils.hpp"
#include <cstdio>    // togliere 
#include <ncurses.h>

void Renderer::init_colors() {
    start_color();
    use_default_colors();

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
    init_pair(CP_UNBREAKABLE_WALL, COLOR_WHITE, COLOR_WHITE);
    init_pair(CP_BREAKABLE_WALL, grey, grey);
    init_pair(CP_DOOR, COLOR_GREEN, bg);
    init_pair(CP_PLAYER, COLOR_CYAN, bg);
    init_pair(CP_ENEMY, COLOR_RED, bg);
    init_pair(CP_BOMB, COLOR_RED, bg);
    init_pair(CP_EXPLOSION, COLOR_YELLOW, COLOR_RED);
    init_pair(CP_BLINK, COLOR_WHITE, bg);
    init_pair(CP_ITEM, COLOR_MAGENTA, bg);
    init_pair(CP_LIFE, COLOR_RED, bg);
    init_pair(CP_TITLE, COLOR_WHITE, bg);
}

Renderer::Renderer() {
    getmaxyx(stdscr, max_y, max_x);

    map_start_y = (max_y - MAP_HEIGHT) / 2;
    map_start_x = (max_x - MAP_WIDTH) / 2;

    map_window = newwin(MAP_HEIGHT, MAP_WIDTH, map_start_y, map_start_x);
    info_window = newwin(MAP_HEIGHT, 20, map_start_y, map_start_x + MAP_WIDTH + 2);

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
    int banner_w = 61;  // larghezza delle righe di TITLE_BANNER
    int top = map_start_y - 2 - TITLE_ROWS;  // sopra la riga HUD, con una riga vuota

    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    if (top >= 0 && max_x >= banner_w) {
        // Centrato rispetto alla griglia (che a sua volta e' centrata)
        int left = map_start_x + (MAP_WIDTH - banner_w) / 2;
        if (left < 0) {
            left = 0;
        }
        for (int i = 0; i < TITLE_ROWS; i++) {
            mvprintw(top + i, left, "%s", TITLE_BANNER[i]);
        }
    }
    else if (map_start_y - 3 >= 0) {
        // Terminale piccolo: titolo compatto
        const char* small_title = "B O M B E R M A N";
        int left = (max_x - 17) / 2;
        mvprintw(map_start_y - 3, left, "%s", small_title);
    }
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
}

void Renderer::display_effect(int buff_remaining) {
    move(map_start_y - 1, map_start_x);

    char text[32] = "None";
    if (buff_remaining > 0) {
        int seconds = (buff_remaining + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND;
        snprintf(text, sizeof(text), "Range %ds", seconds);
    }

    // %-9s: padding a destra per cancellare il testo del frame precedente.
    // 9 = length("Range 10s"), la stringa piu' lunga prevista
    printw("EFFETTO: %-9s", text);
}

void Renderer::display_colors_debug() {
    // Info di debug sotto la griglia
    if (map_start_y + MAP_HEIGHT < max_y) {
        move(map_start_y + MAP_HEIGHT, map_start_x);
        printw("COLORS: %d", COLORS);
    }
}

void Renderer::draw_grid(Map& map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Cell c = map.get_cell({y, x});

            switch (c) {
                case BREAKABLE_WALL:
                    mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_BREAKABLE_WALL));
                    break;

                case UNBREAKABLE_WALL:
                    mvwaddch(map_window, y, x, ' ' | COLOR_PAIR(CP_UNBREAKABLE_WALL));
                    break;

                case ENTRANCE:
                    mvwaddch(map_window, y, x, '<' | COLOR_PAIR(CP_DOOR));
                    break;

                case EXIT:
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

            if (bombs[i].is_blinking()) {
                mvwaddch(map_window, p.y, p.x, 'O' | COLOR_PAIR(CP_BLINK));
            }
            else {
                mvwaddch(map_window, p.y, p.x, 'O' | COLOR_PAIR(CP_BOMB));
            }
            
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

void Renderer::draw_dummy_enemies(DummyEnemy* dummy_enemies) {
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position p = dummy_enemies[i].get_position();
            mvwaddch(map_window, p.y, p.x, '?' | COLOR_PAIR(CP_ENEMY));
        }
    }
}

void Renderer::draw_smart_enemies(SmartEnemy* smart_enemies) {
    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position p = smart_enemies[i].get_position();
            mvwaddch(map_window, p.y, p.x, '!' | COLOR_PAIR(CP_ENEMY));
        }
    }
}

void Renderer::draw_explosions(Map& map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map.is_explosion({y, x})) {
                mvwaddch(map_window, y, x, '*' | COLOR_PAIR(CP_EXPLOSION));
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
    draw_dummy_enemies(level.get_dummy_enemies());
    draw_smart_enemies(level.get_smart_enemies());
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
        waddch(info_window, ACS_DIAMOND | COLOR_PAIR(CP_LIFE));
    }

    int bombs = player.get_bomb_slots() - level.get_bomb_count();
    mvwprintw(info_window, 5, 1, "BOMBS:");
    for (int i = 0; i < bombs; i++) {
        waddch(info_window, ' ');
        waddch(info_window, '*');
    }

    mvwprintw(info_window, 7, 1, "SCORE: %d", score);
    mvwprintw(info_window, 9, 1, "TIME: %d", time);

    wnoutrefresh(info_window);
}

void Renderer::render(LevelManager& level_manager, Player& player, int score, int time) {
    Level& level = level_manager.get_current_level();

    //display_title();
    //display_effect(player.get_buff_remaining());
    //display_colors_debug();

    draw_map(level, player);
    draw_info(level, player, score, time);

    doupdate();
}