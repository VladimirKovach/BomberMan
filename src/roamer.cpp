#include "roamer.hpp"
#include "map.hpp"
#include "utils.hpp"

Roamer::Roamer(Position _p, int _speed) : Enemy(_p, _speed) {}

void Roamer::move(Map& map) {
    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(p, directions[i]);

        if (can_move_to(map, next)) {
            p = next;
            return;
        }
    }
}

void Roamer::update(Map& map) {
    if (move_timer > 0) {
        move_timer--;
    }

    if (move_timer == 0) {
        move(map);
        move_timer = TICKS_PER_SECOND / speed;
    }
}