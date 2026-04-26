#include "DummyEnemy.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _lives, int _speed) : Character(_p, _lives, DUMMY_ENEMY) {
    speed = _speed;
    move_timer = 1.0 / speed;
    last_move_time = -1.0;  // per convenzione

    directions[0] = UP;
    directions[1] = LEFT;
    directions[2] = DOWN;
    directions[3] = RIGHT;
}


bool DummyEnemy::can_move(double game_timer) {
    return last_move_time - game_timer >= move_timer || last_move_time == -1;
}

// Fisher-Yates shuffle: ordina le direzioni in modo casuale
void DummyEnemy::plan_move() {
    for (int i = DIRECTION_COUNT - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Direction tmp = directions[i];
            directions[i] = directions[j];
            directions[j] = tmp;
        }
    }
}


void DummyEnemy::move(Map& map, double game_timer) {
    Position start_p = p;

    for (int i = 0; i < DIRECTION_COUNT; i++) {
        Position next_p = get_next_position(directions[i]);

        if (!map.is_explosion(next_p) && !map.is_enemy(next_p) && !map.is_door(next_p)) {
            Character::move(map, directions[i]);

            if (!positions_equal(p, start_p)) {
                break;
            }
        }
    }

    last_move_time = game_timer;
}

void DummyEnemy::update(Map& map, double game_timer) {
    if (can_move(game_timer)) {
        plan_move();
        move(map, game_timer);
    }
}