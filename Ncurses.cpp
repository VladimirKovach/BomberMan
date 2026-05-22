#include "Ncurses.hpp"
#include <ncurses.h>

Ncurses::Ncurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

Ncurses::~Ncurses() {
    endwin();
}