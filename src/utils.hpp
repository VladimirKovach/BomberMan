#ifndef UTILS_HPP
#define UTILS_HPP

const int DELAY = 50;  // millisecondi

const int TICKS_PER_SECOND = 1000 / DELAY;

const int DIRECTIONS_COUNT = 4;

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

struct Position {
    int y;
    int x;
};

bool equal(Position a, Position b);

Position next_position(Position p, Direction d);

#endif