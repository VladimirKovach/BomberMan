#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Map.hpp"
#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "SmartEnemy.hpp"
#include "Item.hpp"

const int MAX_BOMBS = 3;
const int MAX_DUMMY_ENEMIES = 5;
const int MAX_SMART_ENEMIES = 5;
const int MAX_ITEMS = 10;

class Level {
protected:
    int difficulty;
    bool completed;

    Map map;

    Bomb bombs[MAX_BOMBS];
    DummyEnemy dummy_enemies[MAX_DUMMY_ENEMIES];
    SmartEnemy smart_enemies[MAX_SMART_ENEMIES];
    Item items[MAX_ITEMS];

public:
    Level(int _difficulty = 1);

    bool is_completed();

    Map& get_map();
    Bomb* get_bombs();
    DummyEnemy* get_dummy_enemies();
    SmartEnemy* get_smart_enemies();
    Item* get_items();

    // Posiziona un item del tipo dato sulla cella p, se c'e' uno slot libero.
    // Restituisce true se l'item e' stato posizionato.
    bool spawn_item(Position p, ItemType type);

    int get_bombs_count();
    bool all_enemies_dead();

    void update_bombs(double game_timer);
    void update_enemies(double game_timer, Position player_p);

    void spawn_enemies();

    void reset();
};

#endif