#include "DummyEnemy.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _lives, int _speed) : Character(_p, _lives, ENEMY) {
    init_directions();
    speed = _speed;
    move_timer = 1.0 / speed;
    last_move_time = 1000.0;  // per convenzione
}

void DummyEnemy::init_directions() {
    directions[0] = UP;
    directions[1] = LEFT;
    directions[2] = DOWN;
    directions[3] = RIGHT;
}

void DummyEnemy::shuffle(Direction a[], int length) {
    for (int i = length - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        Direction tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}


void DummyEnemy::move(Map& map, double timer) {
    if ((last_move_time - timer) >= move_timer) {
        shuffle(directions, DIRECTION_COUNT);

        int i = 0;
        Position start_p = p;

        while (i < DIRECTION_COUNT && positions_equal(p, start_p)) {
            Character::move(map, directions[i]);
            i++;
        }

        last_move_time = timer;
    }
}