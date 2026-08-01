#ifndef GAME_HPP
#define GAME_HPP

#include "level_manager.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "utils.hpp"

// Probabilita' (in percentuale) che un drop di item avvenga
const int WALL_DROP_CHANCE = 25;   // alla rottura di un muro
const int ENEMY_DROP_CHANCE = 50;  // alla morte di un nemico

class Game {
protected:
    bool quit;
    int timer;  // tick rimanenti nella partita
    int score;

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool game_over();
    bool win();

    void spawn_player(bool forward);
    void player_death(Level& level);

    void update_timer();

    bool bomb_under_player();
    void handle_input();
    void handle_collisions();

    // Prova a generare un item di tipo casuale in p con probabilita' 'chance'
    void try_drop_item(Level& level, Position p, int chance);

public:
    Game();

    void run();

    int get_score();
};

#endif