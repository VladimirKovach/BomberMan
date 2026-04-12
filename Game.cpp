#include "Game.hpp"
#include <ncurses.h>

// =====================================================
// spawn_enemies: crea i nemici per il livello corrente.
//
// Il numero di nemici scala con la difficolta':
//   - difficulty 1: 1 nemico
//   - difficulty 2: 2 nemici
//   - difficulty 3: 3 nemici
//   - difficulty 4: 4 nemici
//   - difficulty 5: 5 nemici
//
// Ogni nemico viene piazzato su una cella vuota casuale.
// Dopo lo spawn, informiamo la mappa di quanti nemici ci sono
// cosi' puo' sapere quando sono tutti morti (per aprire la porta).
// =====================================================
void Game::spawn_enemies() {
    Map& map = level_manager.get_current_map();
    int difficulty = level_manager.get_current_difficulty();

    // Numero di nemici in base alla difficolta'
    enemy_count = difficulty;
    if (enemy_count > MAX_ENEMIES) {
        enemy_count = MAX_ENEMIES;
    }

    for (int i = 0; i < enemy_count; i++) {
        Position spawn_pos = map.get_random_empty_cell();
        enemies[i] = DummyEnemy(spawn_pos, 1, 1);
        map.set_cell_content(spawn_pos, DUMMY_ENEMY);
    }

    Position spawn_p = map.get_random_empty_cell();
    smart_enemy = SmartEnemy(spawn_p, 1, 2);
    map.set_cell_content(spawn_p, SMART_ENEMY);
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

    // Pulisci le bombe dal livello precedente
    bomb_count = 0;

    // Posiziona il giocatore
    Position spawn;
    if (from_prev) {
        spawn = {1, 1};             // in alto a sinistra (vicino alla porta di entrata)
    }
    else {
        spawn = {MAP_COLS - 2, 1};  // in alto a destra (vicino alla porta di uscita)
    }

    // Adesso per spostare il giocatore in una nuova posizione,
    // creiamo un Player nuovo da zero. Ma cosi perdiamo l'informazione sulle
    // sue vite.
    // TODO: fare un metodo nella classe Player tipo 'set_position'
    player = Player(spawn, 1);
    map.set_cell_content(spawn, PLAYER);

    // LIVELLO
    // Se il livello non ha nemici vivi (prima visita), spawnali
    if (enemy_count == 0 && !level_manager.is_current_completed()) {
        spawn_enemies();
    }
    else {
        // Il livello e' gia' stato visitato, i nemici sono gia' sulla mappa.
        // Dobbiamo ricostruire l'array enemies[] leggendo dalla mappa.
        enemy_count = 0;
        for (int y = 1; y < MAP_ROWS - 1 && enemy_count < MAX_ENEMIES; y++) {
            for (int x = 1; x < MAP_COLS - 1 && enemy_count < MAX_ENEMIES; x++) {
                if (map.get_cell_content({x, y}) == DUMMY_ENEMY) {
                    enemies[enemy_count] = DummyEnemy({x, y}, 1, 1);
                    enemy_count++;
                }
            }
        }
    }
}


// =====================================================
// check_door_transition: controlla se il giocatore e'
// su una porta e gestisce il cambio livello.
// =====================================================
void Game::check_door_transition() {
    Map& map = level_manager.get_current_map();
    Position player_pos = player.get_position();
    CellContent cell = map.get_cell_content(player_pos);

    if (cell == DOOR_NEXT && level_manager.has_next_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_pos);

        // Vai al livello successivo
        level_manager.go_to_next_level();

        // Entra nel nuovo livello (arriva da sinistra)
        enter_level(true);
    }
    else if (cell == DOOR_PREV && level_manager.has_prev_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_pos);

        level_manager.go_to_previous_level();

        // Entra nel nuovo livello (arriva da destra)
        enter_level(false);
    }
}


Game::Game() {
    quit = false;
    timer = TIMER_START_VALUE;
    start = steady_clock::now();
    score = 0;

    bomb_count = 0;
    enemy_count = 0;

    // Entra nel primo livello
    enter_level(true);
}


bool Game::game_over() {
    return timer <= 0 || player.is_dead();
}

bool Game::win() {
    return level_manager.all_levels_completed();
}


void Game::update_bombs() {
    Map& map = level_manager.get_current_map();

    for (int i = 0; i < bomb_count; i++) {
        if (bombs[i].is_timer_expired(timer)) {
            bombs[i].explode(map);
        }
    }

    // Rimuovi bombe esplose
    int count = 0;
    for (int i = 0; i < bomb_count; i++) {
        if (bombs[i].is_exploded()) {
            bombs[i].reset();
        }
        else {
            bombs[count] = bombs[i];
            count++;
        }
    }
    bomb_count = count;
}


void Game::update_enemies() {
    Map& map = level_manager.get_current_map();

    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i].can_move(timer)) {
            enemies[i].plan_movement();
            enemies[i].move(map, timer);
        }
    }

    if (!smart_enemy.is_dead() && smart_enemy.can_move(timer)) {
        smart_enemy.update_player_position(player.get_position());
        smart_enemy.plan_movement();
        smart_enemy.move(map, timer);
    }

    // Rimuovi nemici morti
    int count = 0;
    for (int i = 0; i < enemy_count; i++) {
        if (!enemies[i].is_dead()) {
            enemies[count] = enemies[i];
            count++;
        }
    }
    enemy_count = count;

    if (enemy_count == 0) {
        level_manager.mark_current_completed();
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
            if (bomb_count < MAX_BOMBS && player.get_cell_under() != BOMB) {
                bombs[bomb_count].place(player.get_position(), timer);
                bomb_count++;
                player.set_cell_under(BOMB);
            }
            break;

        default:
            break;
    }
}


void Game::handle_collisions() {
    Map& map = level_manager.get_current_map();
    Position player_p = player.get_position();

    // Collisione player con nemici
    for (int i = 0; i < enemy_count; i++) {
        if (positions_equal(player_p, enemies[i].get_position())) {
            player.take_damage();
        }
    }

    if (positions_equal(player_p, smart_enemy.get_position())) {
        player.take_damage();
    }

    // Collisione player con esplosione (funziona parzialmente)
    if (map.get_cell_content(player_p) == EXPLOSION) {
        player.take_damage();
    }

    // Collisione nemici con esplosione (funziona parzialmente)
    for (int i = 0; i < enemy_count; i++) {
        Position enemy_p = enemies[i].get_position();
        if (map.get_cell_content(enemy_p) == EXPLOSION) {
            enemies[i].take_damage();
        }
    }

    if (!smart_enemy.is_dead() && map.get_cell_content(smart_enemy.get_position()) == EXPLOSION) {
        smart_enemy.take_damage();
    }
}


void Game::run() {
    while (!game_over() && !win() && !quit) {
        Map& map = level_manager.get_current_map();  // da mettere come campo

        update_bombs();
        map.update_explosions();

        update_enemies();

        update_timer(start);

        handle_input();

        handle_collisions();

        check_door_transition();
        level_manager.update_doors(enemy_count);

        renderer.draw_level(map, score, timer, level_manager.get_current_level_number());

        napms(50);  // ~20 FPS
    }
}