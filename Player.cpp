#include "Player.hpp"

Player::Player(Position _p, int _lives): Character(_p, _lives, PLAYER) {}

void Player::heal() {
    lives++;
}

void Player::set_under(CellContent content) {
    under = content;
}