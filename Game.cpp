#include "Game.hpp"
#include <ncurses.h>

Game::Game() :
    map(),
    renderer(),
    player(),
    dummy_enemy({map.get_random_empty_cell()}, 1, 1)
{
    quit = false;
    score = 0;
    timer = TIMER_START_VALUE;
    bomb_count = 0;

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
    timer = TIMER_START_VALUE - elapsed;
    if (timer < 0) timer = 0;
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

        case ' ': {
            // Piazza bomba nella posizione attuale del giocatore
            if (bomb_count < MAX_BOMBS) {
                Position player_pos = player.get_position();

                // Controlla che non ci sia già una
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
                    map.set_cell_content(player_pos, BOMB);
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
    // Aggiorna timer di tutte le bombe attive
    for (int i = 0; i < bomb_count; i++) {
        if (bombs[i].is_active()) {
            bombs[i].update_timer();

            if (bombs[i].is_timer_expired()) {
                bombs[i].explode(map);
            }
        }
    }

    // Rimuovi le bombe già esplose (compatta l'array)
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
    Position player_pos = player.get_position();

    // Collisione player con enemy
    if (positions_equal(player_pos, dummy_enemy.get_position())) {
        player.lose_life();
    }

    // Collisione player con esplosione
    if (map.get_cell_content(player_pos) == EXPLOSION) {
        player.lose_life();
    }

    // Collisione enemy con esplosione
    Position enemy_pos = dummy_enemy.get_position();
    if (map.get_cell_content(enemy_pos) == EXPLOSION) {
        score += 100;
        // TODO: segnalare nemico come morto (aggiungere flag is_dead nella classe DummyEnemy)
        // Per ora lo cancello semplicemente
        map.clear_cell(enemy_pos);
    }
}

void Game::run() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    while (!game_over() && !quit) {
        // Aggiorna bombe (countdown + esplosioni)
        update_bombs();

        // Aggiorna le esplosioni sulla mappa (decrementa timer, rimuovi quelle scadute)
        map.update_explosions();

        // Rendering
        renderer.draw_level(map, score, timer);

        // Movimento nemico
        dummy_enemy.move(map, timer);

        // Input giocatore
        int key = getch();
        handle_input(key);

        // Controlla collisioni
        handle_collision();

        // Aggiorna il timer della partita
        update_timer(start);
    }
}