#include "Bomb.hpp"

Bomb::Bomb(Position _pos, int _range, int _timer) {
    pos = _pos;
    range = _range;
    timer = _timer;
}

Position Bomb::get_position() {
    return pos;
}

bool Bomb::is_timer_expired() {
    return timer <= 0;
}

void Bomb::update_timer() {

}

void Bomb::explode() {

}