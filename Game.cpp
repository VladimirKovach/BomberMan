#include "Game.hpp"
#include <cmath>
#include <ncurses.h>

// =====================================================
// spawn_enemies: crea i nemici per il livello corrente.
//
// Il numero di nemici scala con la difficolta' (da sistemare):
//   - difficulty 1: 1 (dummy), 1 (smart)
//   - difficulty 2: 2 (dummy), 2 (smart)
//   - difficulty 3: 4 (dummy), 3 (smart)
//   - difficulty 4: 8 (dummy), 4 (smart)
//   - difficulty 5: 10 (dummy), 5 (smart)
//
// Ogni nemico viene piazzato su una cella vuota casuale.
// =====================================================
void Game::spawn_enemies() {
    Map& map = level_manager.get_current_map();
    int difficulty = level_manager.get_current_difficulty();

    // Numero di nemici in base alla difficolta'
    dummy_enemy_count = pow(2, difficulty - 1);  // da sistemare
    if (dummy_enemy_count > MAX_DUMMY_ENEMIES) {
        dummy_enemy_count = MAX_DUMMY_ENEMIES;
    }
    
    smart_enemy_count = difficulty;
    if (smart_enemy_count > MAX_SMART_ENEMIES) {
        smart_enemy_count = MAX_SMART_ENEMIES;
    }

    for (int i = 0; i < dummy_enemy_count; i++) {
        Position spawn_p = map.get_random_empty_cell();
        dummy_enemies[i] = DummyEnemy(spawn_p, 1, 1);
        map.set_cell_content(spawn_p, DUMMY_ENEMY);
    }

    for (int i = 0; i < smart_enemy_count; i++) {
        Position spawn_p = map.get_random_empty_cell();
        smart_enemies[i] = SmartEnemy(spawn_p, 1, 1);
        map.set_cell_content(spawn_p, SMART_ENEMY);
    }
}

// =====================================================
// enter_level: configura tutto per giocare nel livello corrente.
//
// Parametro from_prev:
//   - true: il giocatore arriva dal livello precedente
//     (e' entrato dalla porta destra -> spawna a sinistra)
//   - false: il giocatore arriva dal livello successivo
//     (e' entrato dalla porta sinistra -> spawna a destra)
//
// Se il livello ha gia' dei nemici vivi (il giocatore ci
// era gia' stato e poi era tornato indietro), NON li
// rispawniamo - la mappa e' rimasta come l'aveva lasciata.
// =====================================================
void Game::enter_level(bool from_prev) {
    Map& map = level_manager.get_current_map();

    Position spawn;
    if (from_prev) {
        spawn = {1, 1};  // vicino alla porta di entrata
    }
    else {
        spawn = {MAP_COLS - 2, 1};  // vicino alla porta di uscita
    }

    player.set_position(spawn);
    map.set_cell_content(spawn, PLAYER);

    if (!level_manager.is_current_visited()) {
        spawn_enemies();
        level_manager.mark_current_visited();
    }
}


// =====================================================
// check_door_transition: controlla se il giocatore e'
// su una porta e gestisce il cambio livello.
// =====================================================
void Game::check_door_transition() {
    Map& map = level_manager.get_current_map();
    Position player_p = player.get_position();
    CellContent cell = map.get_cell_content(player_p);

    if (cell == NEXT_DOOR && level_manager.has_next_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_p);

        // Vai al livello successivo
        level_manager.go_to_next_level();

        // Entra nel nuovo livello (arriva da sinistra)
        enter_level(true);
    }
    else if (cell == PREV_DOOR && level_manager.has_prev_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_p);

        level_manager.go_to_prev_level();

        // Entra nel nuovo livello (arriva da destra)
        enter_level(false);
    }
}


Game::Game() {
    quit = false;
    timer = TIMER_START_VALUE;
    start = steady_clock::now();
    score = 0;

    dummy_enemy_count = 0;
    smart_enemy_count = 0;

    // Entra nel primo livello
    enter_level(true);
}


bool Game::game_over() {
    return timer <= 0 || player.is_dead();
}

bool Game::win() {
    return level_manager.all_levels_completed();
}

bool Game::all_enemies_dead() {
    return dummy_enemy_count + smart_enemy_count == 0;
}

int Game::get_active_bombs() {
    int count = 0;
    for (int i = 0; i < MAX_ACTIVE_BOMBS; i++) {
        if (bombs[i].is_active()) {
            count++;
        }
    }
    return count;
}


void Game::update_bombs() {
    Map& map = level_manager.get_current_map();

    for (int i = 0; i < MAX_ACTIVE_BOMBS; i++) {
        if (bombs[i].is_active()) {
            bombs[i].update(map, timer);
        }
    }
}


void Game::update_enemies() {
    Map& map = level_manager.get_current_map();

    if (!level_manager.is_current_completed()) {
        for (int i = 0; i < dummy_enemy_count; i++) {
            dummy_enemies[i].update(map, timer);
        }

        for (int i = 0; i < smart_enemy_count; i++) {
            smart_enemies[i].update(map, timer, player.get_position());
        }

        // Rimuovi nemici morti
        int dummy_count = 0;
        for (int i = 0; i < dummy_enemy_count; i++) {
            if (!dummy_enemies[i].is_dead()) {
                dummy_enemies[dummy_count] = dummy_enemies[i];
                dummy_count++;
            }
        }
        dummy_enemy_count = dummy_count;

        int smart_count = 0;
        for (int i = 0; i < smart_enemy_count; i++) {
            if (!smart_enemies[i].is_dead()) {
                smart_enemies[smart_count] = smart_enemies[i];
                smart_count++;
            }
        }
        smart_enemy_count = smart_count;

        if (all_enemies_dead()) {
            level_manager.mark_current_completed();
        }
    }
}


void Game::update_timer(steady_clock::time_point start) {
    steady_clock::time_point now = steady_clock::now();
    double elapsed = duration<double>(now - start).count();
    timer = TIMER_START_VALUE - elapsed;
    if (timer < 0.0) {
        timer = 0.0;
    }
}


void Game::handle_input() {
    Map& map = level_manager.get_current_map();

    int key = getch();
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
            if (get_active_bombs() < MAX_ACTIVE_BOMBS && player.get_under() != BOMB) {
                int i = 0;
                while (i < MAX_ACTIVE_BOMBS && bombs[i].is_active()) {
                    i++;
                }
                if (i < MAX_ACTIVE_BOMBS) {
                    bombs[i].place(player.get_position(), 1, timer);
                    player.set_under(BOMB);
                }
            }
            break;

        default:  // ERR
            break;
    }
}


void Game::handle_collisions() {
    Map& map = level_manager.get_current_map();

    Position player_p = player.get_position();

    // Collisione player con nemici
    for (int i = 0; i < dummy_enemy_count; i++) {
        if (positions_equal(player_p, dummy_enemies[i].get_position())) {
            player.take_damage();
        }
    }

    for (int i = 0; i < smart_enemy_count; i++) {
        if (positions_equal(player_p, smart_enemies[i].get_position())) {
            player.take_damage();
        }
    }

    // Collisione player con esplosione
    if (map.is_explosion(player_p)) {
        player.take_damage();
    }

    // Collisione nemici con esplosione
    for (int i = 0; i < dummy_enemy_count; i++) {
        Position dummy_enemy_p = dummy_enemies[i].get_position();
        if (map.is_explosion(dummy_enemy_p)) {
            dummy_enemies[i].take_damage();
        }
    }

    for (int i = 0; i < smart_enemy_count; i++) {
        Position smart_enemy_p = smart_enemies[i].get_position();
        if (map.is_explosion(smart_enemy_p)) {
            smart_enemies[i].take_damage();
        }
    }

    // Collisione bombe con esplosione
    for (int i = 0; i < MAX_ACTIVE_BOMBS; i++) {
        Position bomb_p = bombs[i].get_position();
        if (bombs[i].is_active() && !bombs[i].is_exploded() && map.is_explosion(bomb_p)) {
            bombs[i].explode(map, timer);
        }
    }
}


void Game::run() {
    while (!game_over() && !win() && !quit) {
        Map& map = level_manager.get_current_map();  // da mettere come campo

        update_bombs();

        update_enemies();

        update_timer(start);

        handle_input();

        handle_collisions();

        check_door_transition();
        level_manager.update_doors();

        renderer.draw_level(map, score, timer, level_manager.get_current_level_number());

        napms(50);  // ~20 FPS
    }
}