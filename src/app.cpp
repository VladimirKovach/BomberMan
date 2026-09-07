#include "app.hpp"
#include "game.hpp"
#include "menu.hpp"
#include <ncurses.h>
#include <iostream>

using namespace std;

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
bool App::terminal_too_small() {
    int lines, cols;
    getmaxyx(stdscr, lines, cols);

    if (lines >= MIN_LINES && cols >= MIN_COLS) {
        return false;
    }
    else {
        endwin();

        // Uso cout invece di printw per evitare di scrivere fuori dallo schermo
        // (Non ho la certezza di avere abbastanza spazio per il messaggio di errore)
        cout << "Terminale troppo piccolo.\n";
        cout << "Dimensione minima: " << MIN_LINES << " x " << MIN_COLS << " (righe x colonne).\n";
        cout << "Dimensione corrente: " << lines << " x " << cols << " (righe x colonne).\n";
        cout << "Ingrandisci la finestra e riavvia il gioco.\n";

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
                // Game viene costruito qui dentro. La pulizia viene fatta da Game::run() prima di uscire.
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