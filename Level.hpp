#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Grid.hpp"
#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "SmartEnemy.hpp"

const int MAX_BOMBS = 3;
const int MAX_DUMMY_ENEMIES = 5;
const int MAX_SMART_ENEMIES = 5;

class Level {
protected:
    int number;
    bool completed;
    bool visited;

    Grid grid;

    Bomb bombs[MAX_BOMBS];
    DummyEnemy dummy_enemies[MAX_DUMMY_ENEMIES];
    SmartEnemy smart_enemies[MAX_SMART_ENEMIES];

public:
    Level(int level_number = 1);

    int get_number();
    bool is_completed();

    Grid& get_grid();
    Bomb* get_bombs();
    DummyEnemy* get_dummy_enemies();
    SmartEnemy* get_smart_enemies();

    int get_bombs_count();
    bool all_enemies_dead();

    void update_bombs(double game_timer);
    void update_enemies(double game_timer, Position player_p);

    void spawn_enemies();
};

#endif