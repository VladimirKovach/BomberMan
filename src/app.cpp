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
    keypad(stdscr, true);
    nodelay(stdscr, true);
}

App::~App() {
    endwin();
}

void App::run() {
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
}