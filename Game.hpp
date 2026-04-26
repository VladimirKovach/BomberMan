#ifndef GAME_HPP
#define GAME_HPP

#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "LevelManager.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "SmartEnemy.hpp"
#include <chrono>
using namespace std::chrono;

const double TIMER_START_VALUE = 1000.0;  // secondi
const int MAX_DUMMY_ENEMIES = 10;  // Numero massimo di nemici (dummy) per livello
const int MAX_SMART_ENEMIES = 5;  // Numero massimo di nemici (smart) per livello
const int MAX_BOMBS = 5;  // Numero massimo di bombe piazzabili contemporaneamente

class Game {
protected:
    bool quit;
    double timer;
    steady_clock::time_point start;
    int score;

    // Array di bombe attive
    Bomb bombs[MAX_BOMBS];
    int bomb_count;

    // Array di nemici vivi
    DummyEnemy dummy_enemies[MAX_DUMMY_ENEMIES];
    int dummy_enemy_count;
    SmartEnemy smart_enemies[MAX_SMART_ENEMIES];
    int smart_enemy_count;

    LevelManager level_manager;
    Renderer renderer;
    Player player;

    bool game_over();

    bool win();

    bool all_enemies_dead();

    void spawn_enemies();

    void update_bombs();

    void update_enemies();

    void update_timer(steady_clock::time_point start);

    void handle_input();

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