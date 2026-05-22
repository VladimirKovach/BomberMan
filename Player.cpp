#include "Player.hpp"

Position get_next_position(Position p, Direction d) {
    Position next_p = p;

    switch (d) {
        case UP:
            (next_p.y)--;
            break;

        case LEFT:
            (next_p.x)--;
            break;

        case DOWN:
            (next_p.y)++;
            break;

        case RIGHT:
            (next_p.x)++;
            break;

        default:  // NONE
            break;
    }

    return next_p;
}


Player::Player(Position _p, int _lives): Character(_p, _lives) {}

void Player::heal() {
    lives++;
}

void Player::move(Grid& grid, Direction d) {
    Position next_p = get_next_position(p, d);
    if (grid.is_walkable(next_p)) {
        p = next_p;
    }
}