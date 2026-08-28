#include "player.hpp"
#include "item.hpp"
#include "map.hpp"
#include "utils.hpp"

Player::Player(Position _p, int _lives) {
    p = _p;
    start_p = _p;

    lives = _lives;

    bomb_range = 1;
    buff_timer = 0;
}

Position Player::get_position() {
    return p;
}

void Player::set_position(Position _p) {
    p = _p;
}

void Player::set_spawn(Position _p) {
    p = _p;
    start_p = _p;
}

int Player::get_lives() {
    return lives;
}

void Player::gain_life() {
    if (lives < MAX_LIVES) {
        lives++;
    }
}

void Player::lose_life() {
    if (lives > 0) {
        lives--;
    }
}

bool Player::is_dead() {
    return lives == 0;
}

bool Player::can_move_to(Map& map, Position next) {
    return !map.out_of_bounds(next) && !map.is_wall(next) && !map.is_bomb(next);
}

void Player::move(Map& map, Direction d) {
    Position next = next_position(p, d);

    if (can_move_to(map, next)) {
        p = next;
    }
}

int Player::get_bomb_range() {
    return bomb_range;
}

// Raccogliere un secondo item raggio non aumenta il bonus:
// prolunga la durata di quello gia' attivo
void Player::apply_buff(int duration) {
    bomb_range = 1 + RANGE_BONUS;
    buff_timer += duration;
}

void Player::update_buff() {
    if (buff_timer > 0) {
        buff_timer--;

        if (buff_timer == 0) {
            bomb_range = 1;
        }
    }
}

int Player::get_buff_remaining() {
    return buff_timer;
}

void Player::reset() {
    p = start_p;
    bomb_range = 1;
    buff_timer = 0;
}