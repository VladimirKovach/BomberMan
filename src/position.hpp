#ifndef POSITION_HPP
#define POSITION_HPP

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

const int DIRECTIONS_COUNT = 4;

struct Position {
    int y;
    int x;
};

// Posizione non valida, fuori dalla mappa
const Position POSITION_NONE = {-1, -1};

bool equal(Position a, Position b);

Position next_position(Position p, Direction d);

#endif