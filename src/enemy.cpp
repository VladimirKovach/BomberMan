#include "enemy.hpp"
#include <cstdlib>

Enemy::Enemy(Position p, int speed, EnemyType type) {
    _directions[0] = UP;
    _directions[1] = LEFT;
    _directions[2] = DOWN;
    _directions[3] = RIGHT;

    _p = p;
    _start_p = p;

    if (speed <= 0) {
        speed = 1;
    }

    _speed = speed;
    _start_speed = speed;
    _move_timer = TICKS_PER_SECOND / _speed;

    _dead = equal(p, {-1, -1});

    _type = type;

    _direction = (Direction) (rand() % DIRECTIONS_COUNT);
}

// distanza di Manhattan
int Enemy::distance(Position a, Position b) {
    return abs(a.y - b.y) + abs(a.x - b.x);
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

// selection sort
void Enemy::sort_directions(Position player_p) {
    for (int i = 0; i < DIRECTIONS_COUNT - 1; i++) {
        int min = i;

        for (int j = i + 1; j < DIRECTIONS_COUNT; j++) {
            Position p_min = next_position(_p, _directions[min]);
            Position p = next_position(_p, _directions[j]);

            if (distance(p, player_p) < distance(p_min, player_p)) {
                min = j;
            }
        }

        if (min != i) {
            Direction tmp = _directions[i];
            _directions[i] = _directions[min];
            _directions[min] = tmp;
        }
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

void Enemy::kill() {
    _dead = true;
}

bool Enemy::is_dead() {
    return _dead;
}

EnemyType Enemy::get_type() {
    return _type;
}

bool Enemy::can_move_to(Map& map, Position p) {
    if (map.out_of_bounds(p)) {
        return false;
    }

    return !map.is_wall(p) && !map.is_bomb(p) && !map.is_explosion(p);
}

void Enemy::update(Map& map, Position player_p) {
    if (_move_timer > 0) {
        _move_timer--;
    }

    if (_move_timer == 0) {
        move(map, player_p);
        _move_timer = TICKS_PER_SECOND / _speed;
    }
}

void Enemy::reset() {
    _p = _start_p;
    _speed = _start_speed;
    _move_timer = TICKS_PER_SECOND / _speed;
    _dead = equal(_p, {-1, -1});
}

void Enemy::move_walker(Map& map) {
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

void Enemy::move_roamer(Map& map) {
    shuffle_directions();

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(_p, _directions[i]);

        if (can_move_to(map, next)) {
            _p = next;
            return;
        }
    }
}

void Enemy::move_chaser(Map& map, Position player_p) {
    sort_directions(player_p);

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        Position next = next_position(_p, _directions[i]);

        if (can_move_to(map, next)) {
            _p = next;
            return;
        }
    }
}

void Enemy::move(Map& map, Position player_p) {
    switch (_type) {
        case ROAMER:
            move_roamer(map);
            break;

        case CHASER:
            move_chaser(map, player_p);
            break;

        // WALKER
        default:
            move_walker(map);
            break;
    }
}