#include "Game.hpp"
#include "Menu.hpp"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

int main() {
    srand(time(0));
    init_ncurses();

    Menu menu;
    bool running = true;

    // Loop principale dell'applicazione: il menu compare ogni volta che finisce una partita
    while (running) {
        MenuChoice choice = menu.show();

        switch (choice) {
            case NEW_GAME: {
                // Game viene costruito qui dentro, a fine partita il distruttore fa pulizia automaticamente.
                // La prossima volta costruisce un Game nuovo di zecca, senza bisogno di un reset() esplicito.
                Game game;
                game.run();
                //if (game.get_score() > 0) {
                    menu.prompt_save_score(game.get_score());
                    menu.show_leaderboard();
                //}
                break;
            }

            case QUIT:
                running = false;
                break;

            case LEADERBOARD:
                menu.show_leaderboard();
                break;
        }
    }

    endwin();
    return 0;
}