#include "app.hpp"
#include "game.hpp"
#include "menu.hpp"
#include <ncurses.h>

App::App() {
    running = true;

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

// Verifica che il terminale sia abbastanza grande.
// Un programma ncurses non puo' ridimensionare la finestra che lo ospita:
// l'unica cosa corretta da fare e' avvisare l'utente e uscire.
bool App::terminal_too_small() {
    int h, w;
    getmaxyx(stdscr, h, w);

    if (h >= MIN_LINES && w >= MIN_COLS) {
        return false;
    }

    mvprintw(0, 0, "Terminale troppo piccolo.");
    mvprintw(1, 0, "Dimensione minima: %d x %d (righe x colonne).", MIN_LINES, MIN_COLS);
    mvprintw(2, 0, "Dimensione attuale: %d x %d.", h, w);
    mvprintw(4, 0, "Ingrandisci la finestra e riavvia il gioco.");
    mvprintw(5, 0, "Premi un tasto per uscire...");

    refresh();

    // il costruttore ha messo nodelay(TRUE): senza questo getch() non aspetterebbe
    nodelay(stdscr, FALSE);
    getch();

    endwin();

    return true;
}

void App::run() {
    if (terminal_too_small()) {
        return;
    }

    // Main Loop: il menu compare ogni volta che finisce una partita
    while (running) {
        MenuChoice choice = menu.show();

        switch (choice) {
            case NEW_GAME:
            {
                // Game viene costruito qui dentro, a fine partita il distruttore fa pulizia automaticamente.
                // La prossima volta costruisce un Game nuovo di zecca, senza bisogno di un reset esplicito.
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
}