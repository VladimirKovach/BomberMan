#ifndef GAME_HPP
#define GAME_HPP

#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "utils.hpp"

const int GAME_TIMER_START = TICKS_PER_SECOND * 300;  // 300 secondi = 5 minuti

// Probabilita' (in percentuale) che un drop di item avvenga
const int WALL_DROP_CHANCE = 25;   // alla rottura di un muro
const int ENEMY_DROP_CHANCE = 50;  // alla morte di un nemico

class Game {
protected:
    bool running;
    int timer;  // tick rimanenti nella partita
    int score;
    int level_score;  // Punteggio al momento dell'ingresso nel livello corrente.
    // Alla morte del giocatore il punteggio torna a questo valore, cosi' i punti fatti nel
    // tentativo fallito non restano acquisiti (il livello viene resettato).

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool win();
    bool lose();

    void handle_player_death(Level& level);

    // Prova a generare un item di tipo casuale in 'p' con probabilita' 'chance'
    void try_drop_item(Level& level, Position p, int chance);

    // Ritorna true se il livello corrente e' cambiato (o e' stato rimosso):
    // in quel caso il Level& del chiamante non e' piu' valido e le altre
    // collisioni vanno saltate fino al frame successivo.
    bool player_doors_collisions(Level& level);
    void player_items_collisions(Level& level);
    void player_enemies_collisions(Level& level);
    void player_explosions_collisions(Level& level);
    void walls_explosions_collisions(Level& level);
    void enemies_explosions_collisions(Level& level);
    void bombs_explosions_collisions(Level& level);
    void handle_collisions();

    void place_bomb(Level& level, Map& map);

    void handle_input();

    void update();

public:
    Game();

    void run();

    int get_score();
};

#endif