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

// algoritmo di shuffle di Fisher-Yates
void DummyEnemy::choose_directions() {
    for (int i = DIRECTION_COUNT - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Direction tmp = directions[i];
            directions[i] = directions[j];
            directions[j] = tmp;
        }
    }
}


void DummyEnemy::move(Map& map, double timer) {
    if ((last_move_time - timer) >= move_timer) {
        choose_directions();

        int i = 0;
        Position start_p = p;

        while (i < DIRECTION_COUNT && positions_equal(p, start_p)) {
            Character::move(map, directions[i]);
            i++;
        }

        last_move_time = timer;
    }
}