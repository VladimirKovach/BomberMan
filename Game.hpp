#ifndef GAME_HPP
#define GAME_HPP

#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "LevelManager.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include <chrono>

// Numero massimo di nemici per livello
const int MAX_ENEMIES = 20;

class Game {
protected:
    const int TIMER_START_VALUE = 1000;  // seconds
    bool quit;
    bool victory;
    int score;
    int timer;
    Bomb bombs[MAX_BOMBS];
    int bomb_count;

    LevelManager levels;
    Renderer renderer;
    Player player;

    // Array di nemici per il livello corrente
    DummyEnemy enemies[MAX_ENEMIES];
    int enemy_count;

    void spawn_enemies();

    // Rimuove tutti i nemici (quando si cambia livello)
    void clear_enemies();

    void update_bombs();

    bool game_over();

    void update_timer(std::chrono::steady_clock::time_point start);

    void handle_input(int key);

    void handle_collision();

    // Gestisce il passaggio tra livelli quando il giocatore entra in una porta
    void check_door_transition();

    // Entra in un livello (piazza il giocatore, spawna nemici)
    void enter_level(bool from_prev);

public:
    Game();

    void run();
};

#endif