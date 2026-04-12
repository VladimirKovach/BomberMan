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
const int MAX_ENEMIES = 10;               // Numero massimo di nemici per livello
const int MAX_BOMBS = 5;                  // Numero massimo di bombe piazzabili contemporaneamente

class Game {
protected:
    bool quit;
    double timer;
    steady_clock::time_point start;
    int score;

    // Array di bombe attive per il livello corrente
    Bomb bombs[MAX_BOMBS];
    int bomb_count;

    // Array di nemici (dummy) per il livello corrente
    DummyEnemy enemies[MAX_ENEMIES];
    int enemy_count;

    SmartEnemy smart_enemy;  // temporaneo

    LevelManager level_manager;
    Renderer renderer;
    Player player;

    bool game_over();

    bool win();

    void spawn_enemies();

    void clear_enemies();  // quando si cambia livello (da implementare)

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