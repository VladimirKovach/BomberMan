#include "roamer.hpp"

Roamer::Roamer(Position p, int speed) : Enemy(p, speed) {}

void Roamer::move(Map& map) {
    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        if (can_move(map, _directions[i])) {
            _p = next_position(_p, _directions[i]);
            return;
        }
    }
}

void Roamer::update(Map& map) {
    if (_move_timer > 0) {
        _move_timer--;
    }

    if (_move_timer == 0) {
        move(map);
        _move_timer = TICKS_PER_SECOND / _speed;
    }
}