#include "roamer.hpp"

Roamer::Roamer(Position p, int speed) : Enemy(p, speed) {}

void Roamer::move(Map& map) {
    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(_p, _directions[i]);
        if (can_move_to(map, next)) {
            _p = next;
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