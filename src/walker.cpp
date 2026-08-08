#include "walker.hpp"
#include <cstdlib>

Walker::Walker(Position p, int speed) : Enemy(p, speed) {
    _direction = (Direction) (rand() % DIRECTIONS_COUNT);
}

void Walker::move(Map& map) {
    Position next = next_position(_p, _direction);

    if (can_move_to(map, next)) {
        _p = next;
        return;
    }

    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(_p, _directions[i]);

        if (can_move_to(map, next)) {
            _p = next;
            _direction = _directions[i];
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