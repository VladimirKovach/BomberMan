#include "SmartEnemy.hpp"
#include <cmath>

SmartEnemy::SmartEnemy(Position _p, int _lives, int _speed) : DummyEnemy(_p, _lives, _speed) {
    type = SMART_ENEMY;  // da sistemare
    player_p = {-1, -1};
    init_pairs();
}

void SmartEnemy::init_pairs() {
    for (int i = 0; i < DIRECTION_COUNT; i++)
        pairs[i].direction = directions[i];
}


// selection sort
void SmartEnemy::sort_pairs() {
    for (int i = 0; i < DIRECTION_COUNT - 1; i++) {
        int min = i;
        for (int j = i + 1; j < DIRECTION_COUNT; j++) {
            if (pairs[j].distance < pairs[min].distance) {
                min = j;
            }
        }

        if (min != i) {
            Pair tmp = pairs[i];
            pairs[i] = pairs[min];
            pairs[min] = tmp;
        }
    }
}


double SmartEnemy::get_distance(Position p1, Position p2) {
    int dx = (p1.x - p2.x);
    int dy = (p1.y - p2.y);
    return sqrt((dx * dx) + (dy * dy));  // togliere sqrt()?
}

void SmartEnemy::update_player_position(Position _player_p) {
    player_p = _player_p;
}


// Ordina le direzioni in base a quanto lo avvicinano al giocatore (greedy)
void SmartEnemy::plan_move() {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        Position next_p = get_next_position(pairs[i].direction);
        pairs[i].distance = get_distance(next_p, player_p);
    }
    
    sort_pairs();
    
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        directions[i] = pairs[i].direction;
    }
}