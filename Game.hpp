#ifndef GAME_HPP
#define GAME_HPP

#include "Enemy.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include <chrono>

class Game {
protected:
    const int TIMER_START_VALUE = 1000;  // seconds
    bool quit;
    int score;
    int timer;

    Enemy enemy;
    Map map;
    Player player;
    Renderer renderer;

    bool game_over();

    void update_timer(std::chrono::steady_clock::time_point start);

    void handle_input(int key);

public:
    Game();

    void run();
};

#endif