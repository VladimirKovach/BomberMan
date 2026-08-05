#include "walker.hpp"
#include <cstdlib>

Walker::Walker(Position p, int speed) : Enemy(p, speed) {
    _d = (Direction) (rand() % DIRECTIONS_COUNT);
}

void Walker::move(Map& map) {
    if (can_move(map, _d)) {
        _p = next_position(_p, _d);
        return;
    }

    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        if (can_move(map, _directions[i])) {
            _d = _directions[i];
            _p = next_position(_p, _d);
            return;
        }
    }
}

void Walker::update(Map& map) {
    if (_move_timer > 0) {
        _move_timer--;
    }

    if (_move_timer == 0) {
        move(map);
        _move_timer = TICKS_PER_SECOND / _speed;
    }
}