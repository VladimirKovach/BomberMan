#include "Player.hpp"

Player::Player(Position _pos, int _lives, int _speed) : Character(_pos, _lives, _speed) {
    can_place_bomb = true;
}

void Player::place_bomb(Map& map) {

}