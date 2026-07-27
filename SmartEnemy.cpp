#include "SmartEnemy.hpp"
#include <cstdlib>

SmartEnemy::SmartEnemy(Position _p, int _speed) : DummyEnemy(_p, _speed) {
    player_p = {-1, -1};
}

void SmartEnemy::update_player_position(Position _player_p) {
    player_p = _player_p;
}

// Distanza di Manhattan
double SmartEnemy::get_player_distance(Position _p) {
    int dx = (_p.x - player_p.x);
    int dy = (_p.y - player_p.y);
    return abs(dx) + abs(dy);
}


// selection sort: ordina le posizioni adiacenti in base alla distanza dal
// giocatore (strategia greedy)
void SmartEnemy::plan_move() {
    for (int i = 0; i < NEIGHBORS_COUNT - 1; i++) {
        int min = i;

        for (int j = i + 1; j < NEIGHBORS_COUNT; j++) {
            Position min_p = neighbors[min];
            Position np = neighbors[j];
            if (get_player_distance(np) < get_player_distance(min_p)) {
                min = j;
            }
        }

        if (min != i) {
            Position tmp = neighbors[i];
            neighbors[i] = neighbors[min];
            neighbors[min] = tmp;
        }
    }
}


// Evita esplosioni
bool SmartEnemy::is_valid_move(Map& map, Position _p) {
    return DummyEnemy::is_valid_move(map, _p) && !map.is_explosion(_p);
}

void SmartEnemy::move(Map& map, double game_timer) {
    for (int i = 0; i < NEIGHBORS_COUNT; i++) {
        if (is_valid_move(map, neighbors[i])) {
            p = neighbors[i];
            last_move_time = game_timer;
            break;
        }
    }
}

void SmartEnemy::update(Map& map, double game_timer, Position _player_p) {
    if (can_move(game_timer)) {
        update_neighbors();
        update_player_position(_player_p);
        plan_move();
        move(map, game_timer);
    }
}