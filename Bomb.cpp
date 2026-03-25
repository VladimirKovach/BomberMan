#include "Bomb.hpp"

Bomb::Bomb(Position _p, int _range, int _timer) {
    p = _p;
    range = _range;
    timer = _timer;
}

Position Bomb::get_position() {
    return p;
}

bool Bomb::is_timer_expired() {
    return timer <= 0;
}

void Bomb::update_timer() {

}

void Bomb::explode() {

}