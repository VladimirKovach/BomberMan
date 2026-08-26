#ifndef GAME_HPP
#define GAME_HPP

#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "utils.hpp"

const int GAME_TIMER_START = TICKS_PER_SECOND * 300;  // 300 secondi = 5 minuti

// Probabilita' (in percentuale) che un drop di item avvenga
const int WALL_DROP_CHANCE = 25;   // alla rottura di un muro
const int ENEMY_DROP_CHANCE = 50;  // alla morte di un nemico

class Game {
protected:
    bool running;
    int timer;  // tick rimanenti nella partita
    int score;

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool win();
    bool lose();

    void handle_player_death(Level& level);

    // Prova a generare un item di tipo casuale in 'p' con probabilita' 'chance'
    void try_drop_item(Level& level, Position p, int chance);

    void player_doors_collisions(Level& level);
    void player_items_collisions(Level& level);
    void player_enemies_collisions(Level& level);
    void player_explosions_collisions(Level& level);
    void walls_explosions_collisions(Level& level);
    void enemies_explosions_collisions(Level& level);
    void bombs_explosions_collisions(Level& level);
    void handle_collisions();

    void place_bomb(Level& level, Map& map);

    void handle_input();

    void update();

public:
    Game();

    void run();

    int get_score();
};

#endif