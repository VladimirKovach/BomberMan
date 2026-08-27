#include "game.hpp"
#include "menu.hpp"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include "map.hpp"

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}


// Spazio minimo richiesto dall'interfaccia:
//   colonne = mappa (41) + separatore (2) + pannello info (15)
//   righe   = mappa (21) + titolo + riga vuota + riga EFFETTO
const int MIN_COLS = MAP_WIDTH + 2 + INFO_WIDTH;
const int MIN_LINES = MAP_HEIGHT + 3;

// Verifica che il terminale sia abbastanza grande. Un programma ncurses non
// puo' ridimensionare la finestra che lo ospita: l'unica cosa corretta da
// fare e' avvisare l'utente e uscire.
bool terminal_too_small() {
    int h, w;
    getmaxyx(stdscr, h, w);

    if (h >= MIN_LINES && w >= MIN_COLS) {
        return false;
    }

    mvprintw(0, 0, "Terminale troppo piccolo.");
    mvprintw(1, 0, "Servono almeno %d colonne x %d righe.", MIN_COLS, MIN_LINES);
    mvprintw(2, 0, "Dimensione attuale: %d x %d.", w, h);
    mvprintw(4, 0, "Ingrandisci la finestra e riavvia il gioco.");
    mvprintw(5, 0, "Premi un tasto per uscire...");

    nodelay(stdscr, FALSE);
    refresh();
    getch();

    return true;
}

int main() {
    srand(time(0));
    init_ncurses();

    if (terminal_too_small()) {
        endwin();
        return 1;
    }

    Menu menu;
    bool running = true;

    // Loop principale dell'applicazione: il menu compare ogni volta che finisce una partita
    while (running) {
        MenuChoice choice = menu.show();

        switch (choice) {
            case NEW_GAME:
            {
                // Game viene costruito qui dentro, a fine partita il distruttore fa pulizia automaticamente.
                // La prossima volta costruisce un Game nuovo di zecca, senza bisogno di un reset() esplicito.
                Game game;
                game.run();
                menu.prompt_save_score(game.get_score());
                menu.show_leaderboard();
                break;
            }

            case LEADERBOARD:
                menu.show_leaderboard();
                break;

            case QUIT:
                running = false;
                break;

            default:
                break;
        }
    }

    endwin();
    return 0;
}
