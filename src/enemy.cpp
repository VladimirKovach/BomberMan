#include "enemy.hpp"
#include "map.hpp"
#include "position.hpp"
#include "time.hpp"
#include <cstdlib>

Enemy::Enemy(Position _p, int _speed) {
    directions[0] = UP;
    directions[1] = LEFT;
    directions[2] = DOWN;
    directions[3] = RIGHT;

    p = _p;
    start_p = _p;
    speed = _speed;

    if (speed <= 0) {
        speed = 1;
    }

    move_timer = TICKS_PER_SECOND / speed;

    dead = equal(p, POSITION_NONE);
}

// Fisher-Yates shuffle
void Enemy::shuffle_directions() {
    for (int i = DIRECTIONS_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        if (j != i) {
            Direction tmp = directions[i];
            directions[i] = directions[j];
            directions[j] = tmp;
        }
    }
}

bool Enemy::can_move_to(Map& map, Position next) {
    return !map.out_of_bounds(next) && !map.is_wall(next) && !map.is_bomb(next) && !map.is_explosion(next);
}

Position Enemy::get_position() {
    return p;
}

void Enemy::kill() {
    dead = true;
}

bool Enemy::is_dead() {
    return dead;
}

void Enemy::reset() {
    p = start_p;
    move_timer = TICKS_PER_SECOND / speed;
    dead = equal(p, POSITION_NONE);
}