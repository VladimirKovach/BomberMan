#ifndef GAME_HPP
#define GAME_HPP

#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "LevelManager.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "SmartEnemy.hpp"
#include <chrono>
using namespace std;

const double TIMER_START_VALUE = 1000.0;  // secondi
const int DELAY = 50;  // millisecondi

// Probabilita' (in percentuale) che un drop di item avvenga
const int WALL_DROP_CHANCE = 25;   // alla rottura di un muro
const int ENEMY_DROP_CHANCE = 50;  // alla morte di un nemico

class Game {
protected:
    bool quit;
    chrono::steady_clock::time_point start;
    double timer;        // tempo rimanente mostrato (include il bonus tempo)
    double game_clock;   // orologio monotono decrescente per bombe, buff e nemici
    double time_bonus;   // secondi accumulati dagli item tempo
    int score;

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool game_over();
    bool win();

    void spawn_player(bool forward);

    void update_timer(chrono::steady_clock::time_point start);

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