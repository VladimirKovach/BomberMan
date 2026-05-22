#include "SmartEnemy.hpp"

SmartEnemy::SmartEnemy(Position _p, int _lives, int _speed) : DummyEnemy(_p, _lives, _speed) {
    player_p = {-1, -1};
}

void SmartEnemy::update_player_position(Position _player_p) {
    player_p = _player_p;
}

// Distanza al quadrato, per confronti non serve sqrt
double SmartEnemy::get_player_distance(Position _p) {
    int dx = (_p.x - player_p.x);
    int dy = (_p.y - player_p.y);
    return (dx * dx) + (dy * dy);
}


// Selection Sort: ordina le posizioni adiacenti in base alla distanza dal
// giocatore (strategia greedy)
void SmartEnemy::plan_move() {
    for (int i = 0; i < ADJACENT_POSITIONS_COUNT - 1; i++) {
        int min = i;

        for (int j = i + 1; j < ADJACENT_POSITIONS_COUNT; j++) {
            Position p_min = adjacent_positions[min];
            Position np = adjacent_positions[j];
            if (get_player_distance(np) < get_player_distance(p_min)) {
                min = j;
            }
        }

        if (min != i) {
            Position tmp = adjacent_positions[i];
            adjacent_positions[i] = adjacent_positions[min];
            adjacent_positions[min] = tmp;
        }
    }
}


// Evita esplosioni
bool SmartEnemy::is_valid_move(Grid& grid, Position _p) {
    return DummyEnemy::is_valid_move(grid, _p) && !grid.is_explosion(_p);
}

void SmartEnemy::move(Grid& grid, double game_timer) {
    for (int i = 0; i < ADJACENT_POSITIONS_COUNT; i++) {
        if (is_valid_move(grid, adjacent_positions[i])) {
            p = adjacent_positions[i];
            last_move_time = game_timer;
            break;
        }
    }
}

void SmartEnemy::update(Grid& grid, double game_timer, Position _player_p) {
    if (can_move(game_timer)) {
        DummyEnemy::update_adjacent_positions();
        update_player_position(_player_p);
        plan_move();
        move(grid, game_timer);
    }
}