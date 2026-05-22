#ifndef GAME_HPP
#define GAME_HPP

#include "Bomb.hpp"
#include "DummyEnemy.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "SmartEnemy.hpp"
#include <chrono>
using namespace std;

const double TIMER_START_VALUE = 1000.0;  // secondi
const int DELAY = 50;  // millisecondi

class Game {
protected:
    bool quit;
    chrono::steady_clock::time_point start;
    double timer;
    int score;

    Map map;
    Player player;
    Renderer renderer;

    bool game_over();
    bool win();

    void spawn_player(bool forward);

    void update_timer(chrono::steady_clock::time_point start);

    bool bomb_under_player();
    void handle_input();
    void handle_collisions();

public:
    Game();

    void run();

    int get_score();
};

#endif