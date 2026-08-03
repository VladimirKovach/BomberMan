#include "smart_enemy.hpp"
#include "dummy_enemy.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <cstdlib>

SmartEnemy::SmartEnemy(Position _p, int _speed) : DummyEnemy(_p, _speed) {}

// distanza di Manhattan
int SmartEnemy::distance(Position a, Position b) {
    return abs(a.y - b.y) + abs(a.x - b.x);
}

// selection sort: ordina i vicini in base alla distanza dal giocatore (greedy)
void SmartEnemy::sort_neighbors(Position player_p) {
    for (int i = 0; i < NEIGHBORS_COUNT - 1; i++) {
        int min = i;

        for (int j = i + 1; j < NEIGHBORS_COUNT; j++) {
            if (distance(neighbors[j], player_p) < distance(neighbors[min], player_p)) {
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

// evita esplosioni
bool SmartEnemy::can_move(Map& map, Position _p) {
    return DummyEnemy::can_move(map, _p) && !map.is_explosion(_p);
}

void SmartEnemy::move(Map& map) {
    for (int i = 0; i < NEIGHBORS_COUNT; i++) {
        if (can_move(map, neighbors[i])) {
            p = neighbors[i];
            return;
        }
    }
}

void SmartEnemy::update(Map& map, Position player_p) {
    if (move_timer > 0) {
        move_timer--;
    }

    if (move_timer == 0) {
        DummyEnemy::update_neighbors();
        sort_neighbors(player_p);
        move(map);

        move_timer = TICKS_PER_SECOND / speed;
    }
}