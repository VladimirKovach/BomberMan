#include "DummyEnemy.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _lives, int _speed) : Character(_p, _lives, _speed) {
    last_move_time = 0;  // by convention
}

Direction DummyEnemy::choose_direction() {
    return (Direction) (rand() % 4);  // UP, LEFT, DOWN, RIGHT
}

void DummyEnemy::move(Map& map, int timer) {
    if (timer != last_move_time) {  // move once per second
        Direction d = choose_direction();
        Character::move(map, d);
        last_move_time = timer;
    }
}