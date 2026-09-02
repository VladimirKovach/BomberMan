#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "bomb.hpp"
#include "chaser.hpp"
#include "item.hpp"
#include "map.hpp"
#include "position.hpp"
#include "roamer.hpp"
#include "walker.hpp"

const int MAX_BOMBS = 3;
const int MAX_ITEMS = 10;

const int MAX_CHASERS = 2;
const int MAX_ROAMERS = 2;
const int MAX_WALKERS = 3;

// Probabilita' (in percentuale) che un drop di item avvenga
const int WALL_DROP_CHANCE = 25;   // alla rottura di un muro
const int ENEMY_DROP_CHANCE = 50;  // alla morte di un nemico

class Level {
protected:
    int number;
    bool completed;

    Map map;

    Bomb bombs[MAX_BOMBS];
    Item items[MAX_ITEMS];

    Chaser chasers[MAX_CHASERS];
    Roamer roamers[MAX_ROAMERS];
    Walker walkers[MAX_WALKERS];

    // Prova a generare un item di tipo casuale in 'p' con probabilita' 'chance'
    void try_drop_item(Position p, int chance);

    int get_bomb_count();
    int get_enemy_count();

    void update_bombs();
    void update_enemies(Position player_p);

    void spawn_enemies();

public:
    Level(int _number = 1);

    int get_number();
    bool is_completed();

    Map& get_map();

    Bomb* get_bombs();
    Item* get_items();

    Chaser* get_chasers();
    Roamer* get_roamers();
    Walker* get_walkers();

    void place_bomb(Position p, int range);

    // Posiziona un item di tipo 'type' in 'p', se c'e' uno slot libero.
    // Restituisce true se l'item e' stato posizionato.
    bool spawn_item(Position p, ItemType type);

    // Distrugge i muri colpiti dalle esplosioni
    // Restituisce il numero di muri distrutti (0 se non ce ne sono)
    int walls_explosions_collisions();

    // Uccide i nemici colpiti dalle esplosioni
    // Restituisce il numero di nemici uccisi (0 se non ce ne sono)
    int chasers_explosions_collisions();
    int roamers_explosions_collisions();
    int walkers_explosions_collisions();

    // Innesca reazione a catena se c'e' collisione
    void bombs_explosions_collisions();

    // Restituisce true se c'e' collisione
    bool player_enemies_collisions(Position player_p);
    bool player_explosions_collisions(Position player_p);

    void update(Position player_p);

    void reset();
};

#endif