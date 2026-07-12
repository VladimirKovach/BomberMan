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


Player::Player(Position _p, int _lives): Character(_p, _lives) {
    start_p = _p;
    bomb_range = 1;
    buff_end = 0.0;
}

void Player::heal() {
    if (lives < MAX_LIVES) {
        lives++;
    }
}

void Player::move(Grid& grid, Direction d) {
    Position next_p = get_next_position(p, d);
    if (grid.is_walkable(next_p)) {
        p = next_p;
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