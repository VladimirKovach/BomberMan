#include "player.hpp"
#include "map.hpp"
#include "utils.hpp"

Player::Player(Position p, int lives) {
    _p = p;
    _start_p = p;

    _lives = lives;

    _bomb_slots = 1;
    _bomb_range = 1;
    _buff_timer = 0;  // nessun buff attivo all'inizio
}

Position Player::get_position() {
    return _p;
}

void Player::set_position(Position p) {
    _p = p;
    _start_p = p;
}

int Player::get_lives() {
    return _lives;
}

void Player::gain_life() {
    if (_lives < MAX_LIVES) {
        _lives++;
    }
}

void Player::lose_life() {
    if (_lives > 0) {
        _lives--;
    }
}

bool Player::is_dead() {
    return _lives == 0;
}

bool Player::can_move_to(Map& map, Position p) {
    return !map.out_of_bounds(p) && !map.is_wall(p) && !map.is_bomb(p);
}

void Player::move(Map& map, Direction d) {
    Position next = next_position(_p, d);
    if (can_move_to(map, next)) {
        _p = next;
    }
}

int Player::get_bomb_slots() {
    return _bomb_slots;
}

void Player::increase_bomb_slots() {
    if (_bomb_slots < MAX_BOMB_SLOTS) {
        _bomb_slots++;
    }
}

int Player::get_bomb_range() {
    return _bomb_range;
}

int Player::get_buff_remaining() {
    return _buff_timer;
}

void Player::apply_range_buff(int duration) {
    // Raccogliere un secondo item raggio non aumenta il bonus:
    // prolunga la durata di quello gia' attivo
    _bomb_range = 1 + RANGE_BONUS;
    _buff_timer += duration;
}

void Player::update_buff() {
    if (_buff_timer > 0) {
        _buff_timer--;

        if (_buff_timer == 0) {
            _bomb_range = 1;
        }
    }
}

void Player::reset() {
    _p = _start_p;
    _bomb_slots = 1;
    _bomb_range = 1;
    _buff_timer = 0;
}