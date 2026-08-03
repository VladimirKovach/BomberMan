#include "dummy_enemy.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _speed) {
    p = _p;
    start_p = _p;

    speed = _speed;
    start_speed = _speed;
    move_timer = TICKS_PER_SECOND / speed;

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

// Fisher-Yates shuffle: ordina i vicini in modo casuale
void DummyEnemy::shuffle_neighbors() {
    for (int i = NEIGHBORS_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        if (j != i) {
            Position tmp = neighbors[i];
            neighbors[i] = neighbors[j];
            neighbors[j] = tmp;
        }
    }
}

bool DummyEnemy::can_move(Map& map, Position _p) {
    return !map.out_of_bounds(_p) && !map.is_wall(_p) && !map.is_door(_p) && !map.is_bomb(_p);
}

void DummyEnemy::move(Map& map) {
    for (int i = 0; i < NEIGHBORS_COUNT; i++) {
        if (can_move(map, neighbors[i])) {
            p = neighbors[i];
            return;
        }
    }
}

void DummyEnemy::update(Map& map) {
    if (move_timer > 0) {
        move_timer--;
    }

    if (move_timer == 0) {
        update_neighbors();
        shuffle_neighbors();
        move(map);

        move_timer = TICKS_PER_SECOND / speed;
    }
}

void DummyEnemy::reset() {
    p = start_p;
    speed = start_speed;
    move_timer = TICKS_PER_SECOND / speed;

    if (equal(p, {-1, -1})) {
        dead = true;
    }
    else {
        dead = false;
    }
}