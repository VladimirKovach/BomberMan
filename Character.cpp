#include "Character.hpp"

Character::Character(Position _p, int _lives) {
    p = _p;
    lives = _lives;
}

void Character::take_damage() {
    lives--;
}

bool Character::is_dead() {
    return lives <= 0;
}

Position Character::get_position() {
    return p;
}

void Character::set_position(Position _p) {
    p = _p;
}

void Character::save_state() {

}

void Character::reset_state() {
    
}