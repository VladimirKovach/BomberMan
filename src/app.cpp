#include "app.hpp"
#include "game.hpp"
#include "menu.hpp"
#include <ncurses.h>
#include <iostream>

App::App() {
    running = true;

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    nodelay(stdscr, true);
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
    else {
        endwin();

        std::cout << "Terminale troppo piccolo." << '\n';
        std::cout << "Dimensione minima: " << MIN_LINES << " x " << MIN_COLS << " (righe x colonne)\n";
        std::cout << "Dimensione attuale: " << h << " x " << w << '\n';
        std::cout << "Ingrandisci la finestra e riavvia il gioco." << '\n';

        return true;
    }
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