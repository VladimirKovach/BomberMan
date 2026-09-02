#include "chaser.hpp"
#include "map.hpp"
#include "position.hpp"
#include "time.hpp"
#include <cstdlib>

// distanza di Manhattan
int Chaser::distance(Position a, Position b) {
    return abs(a.y - b.y) + abs(a.x - b.x);
}

// selection sort
void Chaser::sort_directions(Position player_p) {
    for (int i = 0; i < DIRECTIONS_COUNT - 1; i++) {
        int min = i;

        for (int j = i + 1; j < DIRECTIONS_COUNT; j++) {
            Position p_min = next_position(p, directions[min]);
            Position p_j = next_position(p, directions[j]);

            if (distance(p_j, player_p) < distance(p_min, player_p)) {
                min = j;
            }
        }

        if (min != i) {
            Direction tmp = directions[i];
            directions[i] = directions[min];
            directions[min] = tmp;
        }
    }
}

void Chaser::move(Map& map, Position player_p) {
    sort_directions(player_p);

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(p, directions[i]);

        if (can_move_to(map, next)) {
            p = next;
            return;
        }
    }
}

Chaser::Chaser(Position _p, int _speed) : Enemy(_p, _speed) {}

void Chaser::update(Map& map, Position player_p) {
    if (move_timer > 0) {
        move_timer--;
    }

    if (move_timer == 0) {
        move(map, player_p);
        move_timer = TICKS_PER_SECOND / speed;
    }
}