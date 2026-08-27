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
}

bool Game::win() {
    return level_manager.all_levels_completed();
}

bool Game::lose() {
    return timer == 0 || player.is_dead();
}

void Game::handle_player_death(Level& level) {
    player.lose_life();
    player.reset();
    level.reset();
}

void Game::try_drop_item(Level& level, Position p, int chance) {
    if (rand() % 100 < chance) {
        int r = rand() % 4;

        ItemType type = ITEM_RANGE;

        if (r == 1) {
            type = ITEM_LIFE;
        }
        else if (r == 2) {
            type = ITEM_SCORE;
        }
        else if (r == 3) {
            type = ITEM_TIME;
        }

        level.spawn_item(p, type);
    }
}

void Game::handle_collisions() {

    if (player_doors_collisions(level_manager.get_current_level())) {
        return;
    }

    Level& level = level_manager.get_current_level();

    player_doors_collisions(level);
    player_items_collisions(level);
    player_enemies_collisions(level);
    player_explosions_collisions(level);
    walls_explosions_collisions(level);
    enemies_explosions_collisions(level);
    bombs_explosions_collisions(level);
}

bool Game::player_doors_collisions(Level& level) {
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
        return true;
    }

    return false;
}

void Game::player_items_collisions(Level& level) {
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

void Game::player_enemies_collisions(Level& level) {
    Chaser* chasers = level.get_chasers();
    Roamer* roamers = level.get_roamers();
    Walker* walkers = level.get_walkers();

    Position player_p = player.get_position();

    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            Position chaser_p = chasers[i].get_position();
            if (equal(player_p, chaser_p)) {
                handle_player_death(level);
                return;
            }
        }
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead()) {
            Position roamer_p = roamers[i].get_position();
            if (equal(player_p, roamer_p)) {
                handle_player_death(level);
                return;
            }
        }
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead()) {
            Position walker_p = walkers[i].get_position();
            if (equal(player_p, walker_p)) {
                handle_player_death(level);
                return;
            }
        }
    }
}

void Game::player_explosions_collisions(Level& level) {
    Map& map = level.get_map();

    if (map.is_explosion(player.get_position())) {
        handle_player_death(level);
    }
}

void Game::walls_explosions_collisions(Level& level) {
    Map& map = level.get_map();

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Position p = {y, x};

            if (map.is_explosion(p) && map.is_wall_destructible(p)) {
                map.break_wall(p);
                score++;
                try_drop_item(level, p, WALL_DROP_CHANCE);
            }
        }
    }
}

void Game::enemies_explosions_collisions(Level& level) {
    Map& map = level.get_map();
    Chaser* chasers = level.get_chasers();
    Roamer* roamers = level.get_roamers();
    Walker* walkers = level.get_walkers();

    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            Position chaser_p = chasers[i].get_position();

            if (map.is_explosion(chaser_p)) {
                chasers[i].kill();
                score += 10;
                try_drop_item(level, chaser_p, ENEMY_DROP_CHANCE);
            }
        }
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead()) {
            Position roamer_p = roamers[i].get_position();

            if (map.is_explosion(roamer_p)) {
                roamers[i].kill();
                score += 10;
                try_drop_item(level, roamer_p, ENEMY_DROP_CHANCE);
            }
        }
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead()) {
            Position walker_p = walkers[i].get_position();

            if (map.is_explosion(walker_p)) {
                walkers[i].kill();
                score += 5;
                try_drop_item(level, walker_p, ENEMY_DROP_CHANCE);
            }
        }
    }
}

void Game::bombs_explosions_collisions(Level& level) {
    Map& map = level.get_map();
    Bomb* bombs = level.get_bombs();

    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            Position bomb_p = bombs[i].get_position();

            // Reazione a catena
            if (!bombs[i].is_exploding() && map.is_explosion(bomb_p)) {
                bombs[i].explode(map);
            }
        }
    }
}

void Game::place_bomb(Level& level, Map& map) {
    Bomb* bombs = level.get_bombs();
    Position player_p = player.get_position();

    if (level.get_bomb_count() < MAX_BOMBS && !map.is_bomb(player_p)) {
        int i = 0;

        while (i < MAX_BOMBS && bombs[i].is_active()) {
            i++;
        }

        if (i < MAX_BOMBS) {
            bombs[i].place(map, player_p, player.get_bomb_range());
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
                place_bomb(level, map);
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

    // Il tempo rimanente diventa un bonus per il punteggio
    score += timer / TICKS_PER_SECOND;
}

int Game::get_score() {
    return score;
}