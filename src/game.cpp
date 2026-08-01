#include "game.hpp"
#include "level_manager.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ncurses.h>

void Game::spawn_player(bool forward) {
    Position spawn = {1, 1};  // vicino alla porta di entrata
    if (!forward) {
        spawn = {1, MAP_WIDTH - 2};  // vicino alla porta di uscita
    }
    player.set_position(spawn);
}

Game::Game() {
    quit = false;
    timer = 300 * TICKS_PER_SECOND;  // 300 secondi
    score = 0;
}

bool Game::game_over() {
    return timer <= 0 || player.is_dead();
}

bool Game::win() {
    return level_manager.all_levels_completed();
}

// Il livello riparte com'era: muri, nemici, item e punteggio insieme
void Game::player_death(Level& level) {
    player.lose_life();
    player.reset();
    level.reset();
}

void Game::update_timer() {
    if (timer > 0) {
        timer--;
    }
}

bool Game::bomb_under_player() {
    Bomb* bombs = level_manager.get_current_level().get_bombs();
    for (int i = 0; i < MAX_BOMBS; i++) {
        Position player_p = player.get_position();
        Position bomb_p = bombs[i].get_position();
        if (bombs[i].is_active() && equal(player_p, bomb_p)) {
            return true;
        }
    }
    return false;
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
                quit = true;
                break;

            case ' ':
            {
                Bomb* bombs = level.get_bombs();
                Position player_p = player.get_position();

                if (level.get_bombs_count() < player.get_bomb_slots() && !bomb_under_player()) {
                    int i = 0;

                    while (i < MAX_BOMBS && bombs[i].is_active()) {
                        i++;
                    }

                    if (i < MAX_BOMBS) {
                        bombs[i].place(player_p, player.get_bomb_range());
                    }
                }
                break;
            }

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

        /*
        case '+':
            player.increase_bomb_slots();
            break;
        */

        default:
            break;
    }
}


void Game::handle_collisions() {
    Position player_p = player.get_position();
    Level& level = level_manager.get_current_level();
    Map& map = level.get_map();
    Bomb* bombs = level.get_bombs();
    DummyEnemy* dummy_enemies = level.get_dummy_enemies();
    SmartEnemy* smart_enemies = level.get_smart_enemies();
    Item* items = level.get_items();

    // Collisioni giocatore-porte
    Cell c = map.get_cell(player_p);

    if (c == EXIT && level_manager.has_next_level()) {
        if (level_manager.is_current_completed()) {
            level_manager.remove_current_level(true);
        }
        else {
            level_manager.go_to_next_level();
        }

        spawn_player(true);
    }
    else if (c == ENTRANCE && level_manager.has_prev_level()) {
        if (level_manager.is_current_completed()) {
            level_manager.remove_current_level(false);
        }
        else {
            level_manager.go_to_prev_level();
        }

        spawn_player(false);
    }

    // Raccolta item (il giocatore ci cammina sopra)
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].is_active() && equal(player_p, items[i].get_position())) {
            switch (items[i].get_type()) {
                case ITEM_RANGE:
                    player.apply_range_buff(items[i].get_duration());
                    break;

                case ITEM_LIFE:
                    player.gain_life();
                    break;

                case ITEM_TIME:
                    timer += TIME_BONUS;
                    break;

                case ITEM_SCORE:
                    score += SCORE_BONUS;
                    break;
            }
            items[i].collect();
        }
    }

    // Collisioni giocatore-nemici
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_enemy_p = dummy_enemies[i].get_position();
            if (equal(player_p, dummy_enemy_p)) {
                player_death(level);
                return;
            }
        }
    }

    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_enemy_p = smart_enemies[i].get_position();
            if (equal(player_p, smart_enemy_p)) {
                player_death(level);
                return;
            }
        }
    }

    // Collisioni giocatore-esplosioni
    if (map.is_explosion(player_p)) {
        player_death(level);
        return;
    }

    // Collisioni muri-esplosioni
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Position p = {y, x};
            if (map.is_explosion(p) && map.get_cell(p) == BREAKABLE_WALL) {
                map.set_cell({y, x}, EMPTY);
                score++;
                try_drop_item(level, p, WALL_DROP_CHANCE);
            }
        }
    }

    // Collisioni nemici-esplosioni
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_enemy_p = dummy_enemies[i].get_position();
            if (map.is_explosion(dummy_enemy_p)) {
                dummy_enemies[i].kill();
                score += 3;
                if (dummy_enemies[i].is_dead()) {
                    try_drop_item(level, dummy_enemy_p, ENEMY_DROP_CHANCE);
                }
            }
        }
    }

    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_enemy_p = smart_enemies[i].get_position();
            if (map.is_explosion(smart_enemy_p)) {
                smart_enemies[i].kill();
                score += 5;
                if (smart_enemies[i].is_dead()) {
                    try_drop_item(level, smart_enemy_p, ENEMY_DROP_CHANCE);
                }
            }
        }
    }

    // Collisioni bombe-esplosioni
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


void Game::run() {
    while (!game_over() && !win() && !quit) {
        Level& level = level_manager.get_current_level();

        handle_input();
        handle_collisions();

        update_timer();
        player.update_buff();
        level_manager.update_doors();
        level.update_bombs();
        level.update_enemies(player.get_position());

        renderer.render(level_manager, player, score, timer / TICKS_PER_SECOND);

        napms(DELAY);
    }
}

void Game::try_drop_item(Level& level, Position p, int chance) {
    if (rand() % 100 < chance) {
        // Tipo casuale tra i quattro disponibili
        int r = rand() % 4;
        ItemType type = ITEM_RANGE;
        if (r == 1) {
            type = ITEM_LIFE;
        }
        else if (r == 2) {
            type = ITEM_TIME;
        }
        else if (r == 3) {
            type = ITEM_SCORE;
        }
        level.spawn_item(p, type);
    }
}

int Game::get_score() {
    return score;
}