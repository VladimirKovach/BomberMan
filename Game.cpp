#include "Game.hpp"
#include <curses.h>


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
    Map& map = levels.get_current_map();
    int difficulty = levels.get_current_difficulty();

    // Numero di nemici in base alla difficolta'
    enemy_count = difficulty;
    if (enemy_count > MAX_ENEMIES) {
        enemy_count = MAX_ENEMIES;
    }

    for (int i = 0; i < enemy_count; i++) {
        Position spawn_pos = map.get_random_empty_cell();
        enemies[i] = DummyEnemy(spawn_pos, 1, 1);
        map.set_cell_content(spawn_pos, ENEMY);
    }

    // Comunica alla mappa quanti nemici ci sono
    map.set_enemy_count(enemy_count);
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
    Map& map = levels.get_current_map();

    // Pulisci le bombe dal livello precedente
    bomb_count = 0;

    // Posiziona il giocatore
    Position spawn;
    if (from_prev) {
        spawn = {1, 1};                         // alto sinistra (vicino alla porta entrata)
    } else {
        spawn = {MAP_COLS - 2, MAP_ROWS - 2};   // basso destra (vicino alla porta uscita)
    }

    // Adesso per spostare il giocatore in una nuova posizione,
    // creiamo un Player nuovo da zero. Ma cosi perdiamo l'informazione sulle
    // sue vite.
    // TODO: fare un metodo nella classe Player tipo 'set_position'
    player = Player(spawn, 1, 1);
    map.set_cell_content(spawn, PLAYER);

    // LIVELLO
    // Se il livello non ha nemici vivi (prima visita), spawnali
    if (map.get_alive_enemies() <= 0 && !levels.is_current_completed()) {
        spawn_enemies();
    }
    else {
        // Il livello e' gia' stato visitato, i nemici sono gia' sulla mappa.
        // Dobbiamo ricostruire l'array enemies[] leggendo dalla mappa.
        enemy_count = 0;
        for (int y = 1; y < MAP_ROWS - 1 && enemy_count < MAX_ENEMIES; y++) {
            for (int x = 1; x < MAP_COLS - 1 && enemy_count < MAX_ENEMIES; x++) {
                if (map.get_cell_content({x, y}) == ENEMY) {
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
    Map& map = levels.get_current_map();
    Position player_pos = player.get_position();
    CellContent cell = map.get_cell_content(player_pos);

    if (cell == DOOR_NEXT && levels.has_next_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_pos);

        // Vai al livello successivo
        levels.go_to_next_level();

        // Entra nel nuovo livello (arriva da sinistra)
        enter_level(true);
    }
    else if (cell == DOOR_PREV && levels.has_prev_level()) {
        // Rimuovi il giocatore dalla mappa corrente
        map.clear_cell(player_pos);

        levels.go_to_previous_level();

        // Entra nel nuovo livello (arriva da destra)
        enter_level(false);
    }
}


// =====================================================
// Costruttore
// =====================================================
Game::Game() :
    levels(),
    renderer(),
    player()
{
    quit = false;
    victory = false;
    score = 0;
    timer = TIMER_START_VALUE;
    bomb_count = 0;
    enemy_count = 0;

    // Entra nel primo livello
    enter_level(true);
}


bool Game::game_over() {
    return (timer <= 0 || player.is_dead() || victory);
}

void Game::update_timer(std::chrono::steady_clock::time_point start) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
    timer = TIMER_START_VALUE - elapsed;
    if (timer < 0) timer = 0;
}


void Game::handle_input(int key) {
    Map& map = levels.get_current_map();

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

        case ' ': {
            if (bomb_count < MAX_BOMBS) {
                Position player_pos = player.get_position();

                bool bomb_here = false;
                for (int i = 0; i < bomb_count; i++) {
                    if (bombs[i].is_active() &&
                        positions_equal(bombs[i].get_position(), player_pos)) {
                        bomb_here = true;
                        break;
                    }
                }

                if (!bomb_here) {
                    bombs[bomb_count].place(player_pos, 1, BOMB_DEFAULT_TIMER);
                    player.set_cell_under(BOMB);
                    bomb_count++;
                }
            }
            break;
        }

        default:
            break;
    }
}


void Game::update_bombs() {
    Map& map = levels.get_current_map();

    for (int i = 0; i < bomb_count; i++) {
        if (bombs[i].is_active()) {
            bombs[i].update_timer();

            if (bombs[i].is_timer_expired()) {
                bombs[i].explode(map);
            }
        }
    }

    int write = 0;
    for (int i = 0; i < bomb_count; i++) {
        if (!bombs[i].has_exploded()) {
            if (write != i) {
                bombs[write] = bombs[i];
            }
            write++;
        }
    }
    bomb_count = write;
}


void Game::handle_collision() {
    Map& map = levels.get_current_map();
    Position player_pos = player.get_position();

    // Collisione player con nemici
    for (int i = 0; i < enemy_count; i++) {
        if (!enemies[i].is_dead() &&
            positions_equal(player_pos, enemies[i].get_position())) {
            player.lose_life();
        }
    }

    // Collisione player con esplosione
    if (map.get_cell_content(player_pos) == EXPLOSION) {
        player.lose_life();
    }

    // Collisione nemici con esplosione
    for (int i = 0; i < enemy_count; i++) {
        if (!enemies[i].is_dead()) {
            Position enemy_pos = enemies[i].get_position();
            if (map.get_cell_content(enemy_pos) == EXPLOSION) {
                score += 100;
                enemies[i].lose_life();
                map.enemy_killed();

                // Se tutti i nemici sono morti, segna il livello come completato
                if (map.all_enemies_dead()) {
                    levels.mark_current_completed();
                }
            }
        }
    }
}


// =====================================================
// run: il game loop principale.
// =====================================================
void Game::run() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (!game_over() && !quit) {
        Map& map = levels.get_current_map();

        // 1) Aggiorna bombe
        update_bombs();

        // 2) Aggiorna esplosioni
        map.update_explosions();

        // 3) Aggiorna porte
        levels.update_doors();

        // 4) Rendering
        renderer.draw_level(map, score, timer, levels.get_current_level_number());

        // 5) Muovi nemici
        for (int i = 0; i < enemy_count; i++) {
            if (!enemies[i].is_dead()) {
                enemies[i].move(map, timer);
            }
        }

        // 6) Input giocatore
        int key = getch();
        handle_input(key);

        // 7) Collisioni
        handle_collision();

        // 8) Controlla porte (cambio livello)
        check_door_transition();

        // 9) Controlla vittoria
        if (levels.all_levels_completed()) {
            victory = true;
        }

        // 10) Timer
        update_timer(start);
    }
}