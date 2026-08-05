#include "enemy.hpp"
#include <cstdlib>

Enemy::Enemy(Position p, int speed) {
    _directions[0] = UP;
    _directions[1] = LEFT;
    _directions[2] = DOWN;
    _directions[3] = RIGHT;

    _p = p;
    _start_p = p;

    // controlla speed != 0
    _speed = speed;
    _start_speed = speed;
    _move_timer = TICKS_PER_SECOND / _speed;

    if (equal(p, {-1, -1})) {
        _dead = true;
    }
    else {
        _dead = false;
    }
}

Position Enemy::get_position() {
    return _p;
}

void Enemy::set_position(Position p) {
    _p = p;
}

int Enemy::get_speed() {
    return _speed;
}

void Enemy::set_speed(int speed) {
    _speed = speed;
    _move_timer = TICKS_PER_SECOND / _speed;
}

void Enemy::kill() {
    _dead = true;
}

bool Enemy::is_dead() {
    return _dead;
}

// Fisher-Yates shuffle
void Enemy::shuffle_directions() {
    for (int i = DIRECTIONS_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        if (j != i) {
            Direction tmp = _directions[i];
            _directions[i] = _directions[j];
            _directions[j] = tmp;
        }
    }
}

bool Enemy::can_move(Map& map, Direction d) {
    Position p = next_position(_p, d);

    if (map.out_of_bounds(p)) {
        return false;
    }

    return !map.is_wall(p) && !map.is_bomb(p) && !map.is_explosion(p);
}

void Enemy::reset() {
    _p = _start_p;
    _speed = _start_speed;
    _move_timer = TICKS_PER_SECOND / _speed;

    if (equal(_p, {-1, -1})) {
        _dead = true;
    }
    else {
        _dead = false;
    }
}