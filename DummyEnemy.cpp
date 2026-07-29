#include "DummyEnemy.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _speed) {
    p = _p;
    start_p = _p;

    speed = _speed;
    start_speed = _speed;
    move_timer = 1.0 / speed;  // errore se speed == 0
    last_move_time = -1.0;  // per convenzione

    if (equal(p, {-1, -1})) {
        dead = true;
    }
    else {
        dead = false;
    }

    update_neighbors();
}

Position DummyEnemy::get_position() {
    return p;
}

void DummyEnemy::set_position(Position _p) {
    p = _p;
}

int DummyEnemy::get_speed() {
    return speed;
}

void DummyEnemy::kill() {
    dead = true;
}

bool DummyEnemy::is_dead() {
    return dead == true;
}

void DummyEnemy::update_neighbors() {
    neighbors[0] = {p.y + 1, p.x};
    neighbors[1] = {p.y, p.x - 1};
    neighbors[2] = {p.y - 1, p.x};
    neighbors[3] = {p.y, p.x + 1};
}

bool DummyEnemy::can_move(double game_timer) {
    return last_move_time - game_timer >= move_timer || last_move_time == -1;
}

// Fisher-Yates shuffle: ordina i vicini in modo casuale
void DummyEnemy::plan_move() {
    for (int i = NEIGHBORS_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Position tmp = neighbors[i];
            neighbors[i] = neighbors[j];
            neighbors[j] = tmp;
        }
    }
}

bool DummyEnemy::is_valid_move(Map& map, Position _p) {
    return !map.is_wall(_p) && !map.is_door(_p);
}

void DummyEnemy::move(Map& map, double game_timer) {
    for (int i = 0; i < NEIGHBORS_COUNT; i++) {
        if (is_valid_move(map, neighbors[i])) {
            p = neighbors[i];
            last_move_time = game_timer;
            break;
        }
    }
}

void DummyEnemy::update(Map& map, double game_timer) {
    if (can_move(game_timer)) {
        update_neighbors();
        plan_move();
        move(map, game_timer);
    }
}

void DummyEnemy::reset() {
    p = start_p;
    speed = start_speed;

    if (equal(p, {-1, -1})) {
        dead = true;
    }
    else {
        dead = false;
    }
}