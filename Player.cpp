#include "Player.hpp"

Player::Player(Position _p, int _lives, int _speed) : Character(_p, _lives, _speed) {
    can_place_bomb = true;
}

void Player::place_bomb(Map& map) {

}