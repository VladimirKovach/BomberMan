#ifndef GAME_HPP
#define GAME_HPP

#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "LevelManager.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include <chrono>

const int MAX_ENEMIES = 10;  // Numero massimo di nemici per livello
const int MAX_BOMBS = 3;     // Numero massimo di bombe piazzabili contemporaneamente

class Game {
protected:
    const int TIMER_START_VALUE = 1000;  // seconds
    bool quit;
    int score;
    int timer;

    // Array di bombe attive per il livello corrente
    Bomb bombs[MAX_BOMBS];
    int bomb_count;

    // Array di nemici per il livello corrente
    DummyEnemy enemies[MAX_ENEMIES];
    int enemy_count;

    LevelManager level_manager;
    Renderer renderer;
    Player player;

    void spawn_enemies();

    // Rimuove tutti i nemici (quando si cambia livello)
    void clear_enemies();

    void update_bombs();

    bool game_over();
    bool win();

    void update_timer(std::chrono::steady_clock::time_point start);

    void handle_input();

    void handle_enemy_movement();

    void handle_collisions();

    // Gestisce il passaggio tra livelli quando il giocatore entra in una porta
    void check_door_transition();

    // Entra in un livello (piazza il giocatore, spawna nemici)
    void enter_level(bool from_prev);

public:
    Game();

    void run();
};

#endif