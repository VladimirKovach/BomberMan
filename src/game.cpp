#include "game.hpp"
#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ncurses.h>

Game::Game() {
    running = true;
    timer = GAME_TIMER_START;
    score = 0;
    level_score = 0;
}

bool Game::win() {
    return level_manager.all_levels_completed();
}

bool Game::lose() {
    return timer == 0 || player.is_dead();
}

void Game::handle_collisions() {
    Level& level = level_manager.get_current_level();

    if (handle_level_change(level)) {
        return;
    }

    handle_item_collection(level);

    score += level.walls_explosions_collisions();
    score += 10 * level.chasers_explosions_collisions();
    score += 10 * level.roamers_explosions_collisions();
    score += 5 * level.walkers_explosions_collisions();

    level.bombs_explosions_collisions();

    Position player_p = player.get_position();

    if (level.player_enemies_collisions(player_p) || level.player_explosions_collisions(player_p)) {
        player.lose_life();
        player.reset();
        level.reset();
        score = level_score;
    }
}

bool Game::handle_level_change(Level& level) {
    Map& map = level.get_map();

    Position player_p = player.get_position();

    if (map.is_door_next(player_p) && level_manager.has_next_level()) {
        if (level.is_completed()) {
            level_manager.remove_current_level(true);
        }
        else {
            level_manager.go_to_next_level();
        }

        player.set_position({1, 1});
        level_score = score;

        return true;
    }

    if (map.is_door_prev(player_p) && level_manager.has_prev_level()) {
        if (level.is_completed()) {
            level_manager.remove_current_level(false);
        }
        else {
            level_manager.go_to_prev_level();
        }

        player.set_position({1, MAP_WIDTH - 2});
        level_score = score;

        return true;
    }

    return false;
}

void Game::handle_item_collection(Level& level) {
    Item* items = level.get_items();

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].is_active() && equal(player.get_position(), items[i].get_position())) {
            switch (items[i].get_type()) {
                case ITEM_RANGE:
                    player.apply_buff(items[i].get_duration());
                    break;

                case ITEM_LIFE:
                    player.gain_life();
                    break;

                case ITEM_SCORE:
                    score += SCORE_BONUS;
                    break;

                case ITEM_TIME:
                    timer += TIME_BONUS;
                    break;
            }

            items[i].collect();
        }
    }
}


void Game::handle_input() {
    Level& level = level_manager.get_current_level();
    Map& map = level.get_map();

    // La tastiera in auto-repeat genera piu' eventi di quanti frame
    // consumiamo (~30/s contro ~20 fps), quindi i tasti si accumulano nel
    // buffer e il personaggio continua a muoversi dopo il rilascio.
    // Svuotiamo tutto il buffer: bomba e uscita vengono gestite subito,
    // per il movimento conta solo l'ultimo tasto letto.

    //move_key: è un appunto.
    //Durante la lettura del buffer non muoviamo nessuno — ci segniamo solo l'ultimo tasto di movimento che abbiamo visto.
    //Tre D nel buffer → l'appunto viene sovrascritto tre volte, alla fine dice semplicemente "D".
    //Poi, a buffer vuoto, guardiamo l'appunto e facciamo un movimento.
    int move_key = ERR;
    int key = getch();

    while (key != ERR) {
        switch (key) {
            case 'q':
            case 'Q':
                running = false;
                break;

            case ' ':
                level.place_bomb(player.get_position(), player.get_bomb_range());
                break;

            default:
                move_key = key;
                break;
        }

        key = getch();
    }

    switch (move_key) {
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

        default:
            break;
    }
}

void Game::update() {
    if (timer > 0) {
        timer--;
    }

    player.update_buff();
    level_manager.update_doors();

    Level& level = level_manager.get_current_level();
    level.update(player.get_position());

    handle_collisions();

    if (win() || lose()) {
        running = false;
    }
}

void Game::run() {
    while (running) {
        handle_input();
        update();

        renderer.render(level_manager, player, score, timer / TICKS_PER_SECOND);
        napms(DELAY);
    }

    // Il tempo rimanente diventa un bonus (in caso di vittoria)
    if (win()) {
        score += timer / TICKS_PER_SECOND;
    }
}

int Game::get_score() {
    return score;
}