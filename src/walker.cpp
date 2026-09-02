#include "walker.hpp"
#include "map.hpp"
#include "position.hpp"
#include "time.hpp"
#include <cstdlib>

Walker::Walker(Position _p, int _speed) : Enemy(_p, _speed) {
    direction = (Direction) (rand() % DIRECTIONS_COUNT);
}

void Walker::move(Map& map) {
    Position next = next_position(p, direction);

    if (can_move_to(map, next)) {
        p = next;
        return;
    }

    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(p, directions[i]);

        if (can_move_to(map, next)) {
            p = next;
            direction = directions[i];
            return;
        }
    }
}

void Walker::update(Map& map) {
    if (move_timer > 0) {
        move_timer--;
    }

    if (move_timer == 0) {
        move(map);
        move_timer = TICKS_PER_SECOND / speed;
    }
}