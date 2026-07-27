#include "Game.hpp"
#include <cstdlib>
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
    game_clock = TIMER_START_VALUE;
    time_bonus = 0.0;
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

    // Orologio monotono decrescente (senza bonus): riferimento stabile per
    // bombe, buff e nemici, che NON deve saltare quando si raccoglie un item tempo
    game_clock = TIMER_START_VALUE - elapsed;

    // Tempo mostrato e usato per vittoria/sconfitta: include il bonus accumulato
    timer = game_clock + time_bonus;
    if (timer < 0.0) {
        timer = 0.0;
    }
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
                if (level.get_bombs_count() < MAX_BOMBS && !bomb_under_player()) {
                    int i = 0;
                    while (i < MAX_BOMBS && bombs[i].is_active()) {
                        i++;
                    }
                    if (i < MAX_BOMBS) {
                        bombs[i].place(player_p, player.get_bomb_range(), game_clock);
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

        default:
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
    Item* items = level.get_items();

    // Raccolta item (il giocatore ci cammina sopra)
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].is_active() && positions_equal(player_p, items[i].get_position())) {
            switch (items[i].get_type()) {
                case ITEM_RANGE:
                    player.apply_range_buff(items[i].get_duration(), game_clock);
                    break;

                case ITEM_LIFE:
                    player.gain_life();
                    break;

                case ITEM_TIME:
                    time_bonus += TIME_BONUS;
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
            if (positions_equal(player_p, dummy_enemy_p)) {
                player.lose_life();
                level.reset();
                player.reset();
            }
        }
    }

    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        if (!smart_enemies[i].is_dead()) {
            Position smart_enemy_p = smart_enemies[i].get_position();
            if (positions_equal(player_p, smart_enemy_p)) {
                player.lose_life();
                level.reset();
                player.reset();
            }
        }
    }

    // Collisioni giocatore-esplosioni
    if (grid.is_explosion(player_p)) {
        player.lose_life();
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
                try_drop_item(level, p, WALL_DROP_CHANCE);
            }
        }
    }

    // Collisioni nemici-esplosioni
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        if (!dummy_enemies[i].is_dead()) {
            Position dummy_enemy_p = dummy_enemies[i].get_position();
            if (grid.is_explosion(dummy_enemy_p)) {
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
            if (grid.is_explosion(smart_enemy_p)) {
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
            if (!bombs[i].is_exploding() && grid.is_explosion(bomb_p)) {
                bombs[i].explode(grid, game_clock);
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
        player.update_buff(game_clock);
        map.update_doors();
        map.update_all_bombs(game_clock);
        level.update_enemies(game_clock, player.get_position());

        renderer.render(map, player, score, timer, game_clock);
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