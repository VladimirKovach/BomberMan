#ifndef POSITION_HPP
#define POSITION_HPP

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

const int DIRECTIONS_COUNT = 4;

struct Position {
    int y;
    int x;
};

const Position POSITION_NONE = {-1, -1};  // posizione non valida, fuori dalla mappa

bool equal(Position a, Position b);

Position next_position(Position p, Direction d);

#endif