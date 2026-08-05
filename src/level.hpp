#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "bomb.hpp"
#include "item.hpp"
#include "map.hpp"
#include "utils.hpp"

#include "roamer.hpp"
#include "walker.hpp"

const int MAX_BOMBS = 3;
const int MAX_ITEMS = 10;

const int MAX_ROAMERS = 5;
const int MAX_WALKERS = 5;

class Level {
protected:
    int number;
    bool completed;

    Map map;

    Bomb bombs[MAX_BOMBS];
    Item items[MAX_ITEMS];

    Roamer roamers[MAX_ROAMERS];
    Walker walkers[MAX_WALKERS];

public:
    Level(int _number = 1);

    int get_number();
    bool is_completed();

    Map& get_map();
    Bomb* get_bombs();
    Item* get_items();
    Roamer* get_roamers();
    Walker* get_walkers();

    // Posiziona un item del tipo dato sulla cella p, se c'e' uno slot libero.
    // Restituisce true se l'item e' stato posizionato.
    bool spawn_item(Position p, ItemType type);

    int get_bomb_count();
    int get_enemy_count();

    void update_bombs();
    void update_enemies(Position player_p);

    void spawn_enemies();

    void reset();
};

#endif