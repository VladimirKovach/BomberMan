#include "DummyEnemy.hpp"
#include <cstdlib>

DummyEnemy::DummyEnemy(Position _p, int _lives, int _speed) : Character(_p, _lives) {
    speed = _speed;
    move_timer = 1.0 / speed;
    last_move_time = -1.0;  // per convenzione

    start_p = _p;
    start_lives = _lives;
    start_speed = _speed;

    update_neighbors();
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

// Fisher-Yates shuffle: ordina le posizioni adiacenti in modo casuale
void DummyEnemy::plan_move() {
    for (int i = NEIGHBORS_COUNT - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        if (j != i) {
            Position tmp = neighbors[i];
            neighbors[i] = neighbors[j];
            neighbors[j] = tmp;
        }
    }
}

// Una mossa e' valida sse la cella di arrivo non e' un muro e non e' una porta
bool DummyEnemy::is_valid_move(Grid& grid, Position _p) {
    return grid.is_walkable(_p) && !grid.is_door(_p);
}

void DummyEnemy::move(Grid& grid, double game_timer) {
    for (int i = 0; i < NEIGHBORS_COUNT; i++) {
        if (is_valid_move(grid, neighbors[i])) {
            p = neighbors[i];
            last_move_time = game_timer;
            break;
        }
    }
}

void DummyEnemy::update(Grid& grid, double game_timer) {
    if (can_move(game_timer)) {
        update_neighbors();
        plan_move();
        move(grid, game_timer);
    }
}

void DummyEnemy::reset() {
    p = start_p;
    lives = start_lives;
    speed = start_speed;
}