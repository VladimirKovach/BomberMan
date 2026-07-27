#include "Player.hpp"

Position Player::next_position(Direction d) {
    Position next = p;

    switch (d) {
        case UP:
            next.y--;
            break;

        case LEFT:
            next.x--;
            break;

        case DOWN:
            next.y++;
            break;

        case RIGHT:
            next.x++;
            break;

        default:
            break;
    }

    return next;
}

Player::Player(Position _p, int _lives) {
    p = _p;
    start_p = _p;
    lives = _lives;
    bomb_range = 1;
    buff_end = 0.0;
}

Position Player::get_position() {
    return p;
}

void Player::set_position(Position _p) {
    p = _p;
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
    return lives <= 0;
}

void Player::move(Grid& grid, Direction d) {
    Position next = next_position(d);
    if (grid.is_walkable(next)) {
        p = next;
    }
}

int Player::get_bomb_range() {
    return bomb_range;
}

double Player::get_buff_remaining(double game_clock) {
    // Orologio decrescente: il buff e' attivo finche' game_clock > buff_end
    if (bomb_range > 1 && game_clock > buff_end) {
        return game_clock - buff_end;
    }
    return 0.0;
}

void Player::apply_range_buff(double duration, double game_clock) {
    // L'orologio di gioco e' decrescente: scadenza piu' bassa = buff piu' lungo.
    if (bomb_range > 1) {
        // Buff gia' attivo: la durata si somma (il raggio resta al bonus singolo)
        buff_end -= duration;
    }
    else {
        bomb_range = 1 + RANGE_BONUS;
        buff_end = game_clock - duration;
    }
}

void Player::update_buff(double game_clock) {
    // Se il buff e' attivo ed e' scaduto, torno al raggio base
    if (bomb_range > 1 && game_clock <= buff_end) {
        bomb_range = 1;
    }
}

void Player::reset() {
    p = start_p;
    bomb_range = 1;  // morendo si perde anche il buff raggio
}