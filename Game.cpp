#include "Game.hpp"
#include <algorithm>
#include <ncurses.h>
using namespace std;

void Game::spawn_player(bool forward) {
    Position spawn = {1, 1};  // vicino alla porta di entrata
    if (!forward) {
        spawn = {1, GRID_COLS - 2};  // vicino alla porta di uscita
    }
    player.set_position(spawn);
}

Game::Game() {
    quit = false;
    start = chrono::steady_clock::now();
    timer = TIMER_START_VALUE;
    score = 0;

    // Entra nel primo livello
    spawn_player(true);
}


bool Game::game_over() {
    return timer <= 0 || player.is_dead();
}

bool Game::win() {
    return map.all_levels_completed();
}


void Game::update_timer(chrono::steady_clock::time_point start) {
    chrono::steady_clock::time_point now = chrono::steady_clock::now();
    double elapsed = chrono::duration<double>(now - start).count();
    timer = max(0.0, TIMER_START_VALUE - elapsed);
}

bool Game::bomb_under_player() {
    Bomb* bombs = map.get_current_level().get_bombs();
    for (int i = 0; i < MAX_BOMBS; i++) {
        Position player_p = player.get_position();
        Position bomb_p = bombs[i].get_position();
        if (bombs[i].is_active() && positions_equal(player_p, bomb_p)) {
            return true;
        }
    }
    return false;
}


void Game::handle_input() {
    Level& level = map.get_current_level();
    Grid& grid = level.get_grid();

    int key = getch();
    switch (key) {
        case 'q':
        case 'Q':
            quit = true;
            break;

        case KEY_UP:
        case 'w':
        case 'W':
            player.move(grid, UP);
            break;

        case KEY_LEFT:
        case 'a':
        case 'A':
            player.move(grid, LEFT);
            break;

        case KEY_DOWN:
        case 's':
        case 'S':
            player.move(grid, DOWN);
            break;

        case KEY_RIGHT:
        case 'd':
        case 'D':
            player.move(grid, RIGHT);
            break;

        case ' ':
        {
            Bomb* bombs = level.get_bombs();
            Position player_p = player.get_position();
            if (level.get_bombs_count() < MAX_BOMBS && !bomb_under_player()) {
                int i = 0;
                while (i < MAX_BOMBS && bombs[i].is_active()) {
                    i++;
                }
                if (i < MAX_BOMBS) {
                    bombs[i].place(player_p, 1, timer);
                }
            }
            break;
        }

        default:  // ERR
            break;
    }
}


void Game::handle_collisions() {
    Position player_p = player.get_position();
    Level& level = map.get_current_level();
    Grid& grid = level.get_grid();
    Bomb* bombs = level.get_bombs();
    DummyEnemy* dummy_enemies = level.get_dummy_enemies();
    SmartEnemy* smart_enemies = level.get_smart_enemies();

    // Collisioni giocatore-nemici
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_enemy_p = dummy_enemies[i].get_position();
            if (positions_equal(player_p, dummy_enemy_p)) {
                player.take_damage();
                level.reset();
                player.reset();
            }
        }
    }

    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_enemy_p = smart_enemies[i].get_position();
            if (positions_equal(player_p, smart_enemy_p)) {
                player.take_damage();
                level.reset();
                player.reset();
            }
        }
    }

    // Collisioni giocatore-esplosioni
    if (grid.is_explosion(player_p)) {
        player.take_damage();
        level.reset();
        player.reset();
    }

    // Collisioni muri-esplosioni
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            Position p = {y, x};
            if (grid.is_explosion(p) && grid.get_cell(p) == BREAKABLE_WALL) {
                grid.set_cell({y, x}, EMPTY);
                score++;
            }
        }
    }

    // Collisioni nemici-esplosioni
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_enemy_p = dummy_enemies[i].get_position();
            if (grid.is_explosion(dummy_enemy_p)) {
                dummy_enemies[i].take_damage();
                score += 3;
            }
        }
    }

    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_enemy_p = smart_enemies[i].get_position();
            if (grid.is_explosion(smart_enemy_p)) {
                smart_enemies[i].take_damage();
                score += 5;
            }
        }
    }

    // Collisioni bombe-esplosioni
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            Position bomb_p = bombs[i].get_position();
            // Reazione a catena
            if (!bombs[i].is_exploding() && grid.is_explosion(bomb_p)) {
                bombs[i].explode(grid, timer);
            }
        }
    }

    // Collisioni giocatore-porte
    Cell c = grid.get_cell(player_p);
    if (c == EXIT && map.has_next_level()) {
        if (map.is_current_completed()) {
            map.remove_current_level(true);
        }
        else {
            map.go_to_next_level();
        }
        spawn_player(true);
    }
    else if (c == ENTRANCE && map.has_prev_level()) {
        if (map.is_current_completed()) {
            map.remove_current_level(false);
        }
        else {
            map.go_to_prev_level();
        }
        spawn_player(false);
    }
}


void Game::run() {
    while (!game_over() && !win() && !quit) {
        Level& level = map.get_current_level();

        handle_input();
        handle_collisions();

        update_timer(start);
        map.update_doors();
        map.update_all_bombs(timer);
        level.update_enemies(timer, player.get_position());

        renderer.render(map, player.get_position(), score, timer);
        napms(DELAY);
    }
}

int Game::get_score() {
    return score;
}