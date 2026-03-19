#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(Position _pos, int _lives, int _speed) : Character(_pos, _lives, _speed) {
    last_move_time = 0;  // by convention
}

Direction Enemy::choose_direction() {
    return (Direction) (rand() % 4);  // UP, LEFT, DOWN, RIGHT
}

void Enemy::move(Map& map, int timer) {
    if (timer != last_move_time) {  // move once per second
        Direction dir = choose_direction();
        Character::move(map, dir);
        last_move_time = timer;
    }
}