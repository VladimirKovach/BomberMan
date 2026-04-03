#include "Game.hpp"
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

    Game game;
    game.run();

    endwin();

    return 0;
}