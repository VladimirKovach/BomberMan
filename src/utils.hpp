#ifndef UTILS_HPP
#define UTILS_HPP

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