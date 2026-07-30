#include "utils.hpp"

bool equal(Position a, Position b) {
    return a.y == b.y && a.x == b.x;
}

Position next_position(Position p, Direction d) {
    switch (d) {
        case UP:
            p.y--;
            break;

        case LEFT:
            p.x--;
            break;

        case DOWN:
            p.y++;
            break;

        case RIGHT:
            p.x++;
            break;

        default:
            break;
    }

    return p;
}