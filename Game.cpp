#include "Game.hpp"
#include <algorithm>
#include <ncurses.h>

Game::Game() :
    map(),
    renderer(),
    player(),
    dummy_enemy({map.get_random_empty_cell()}, 1, 1)
{
    quit = false;
    score = 0;  // da implementare
    timer = TIMER_START_VALUE;

    // spawn characters
    map.set_cell_content(player.get_position(), PLAYER);
    map.set_cell_content(dummy_enemy.get_position(), ENEMY);
}


bool Game::game_over() {
    return (timer <= 0 || player.is_dead());
}

void Game::update_timer(std::chrono::steady_clock::time_point start) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
    timer = std::max(0, TIMER_START_VALUE - elapsed);  // use max to prevent negative timer
}


void Game::handle_input(int key) {
    switch (key) {
        case 'q':
        case 'Q':
            quit = true;
            break;

        case KEY_UP:
        case 'w':
        case 'W':
            player.move(map, UP);
            break;

        case KEY_LEFT:
        case 'a':
        case 'A':
            player.move(map, LEFT);
            break;

        case KEY_DOWN:
        case 's':
        case 'S':
            player.move(map, DOWN);
            break;

        case KEY_RIGHT:
        case 'd':
        case 'D':
            player.move(map, RIGHT);
            break;

        case ' ':
            //player.place_bomb(map);
            break;

        default:
            break;
    }
}


void Game::handle_collision() {
    if (positions_equal(player.get_position(), dummy_enemy.get_position())) {
        player.lose_life();
    }
}


void Game::run() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (!game_over() && !quit) {
        renderer.draw_level(map, score, timer);
        
        dummy_enemy.move(map, timer);

        int key = getch();
        handle_input(key);

        handle_collision();

        update_timer(start);
    }
}