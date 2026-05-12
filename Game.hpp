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
const int MAX_ACTIVE_BOMBS = 3;

class Game {
protected:
    bool quit;
    double timer;
    steady_clock::time_point start;
    int score;

    Bomb bombs[MAX_ACTIVE_BOMBS];

    LevelManager level_manager;
    Renderer renderer;
    Player player;

    bool game_over();
    bool win();

    bool all_enemies_dead();

    int get_active_bombs();

    void update_bombs();

    void update_enemies();

    void update_timer(steady_clock::time_point start);

    void handle_input();

    void handle_collisions();

    void spawn_enemies();

    // Gestisce il passaggio tra livelli quando il giocatore entra in una porta
    void check_door_transition();

    // Entra in un livello (piazza il giocatore, spawna nemici)
    void enter_level(bool from_prev);

public:
    Game();

    void run();
};

#endif