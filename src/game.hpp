#ifndef GAME_HPP
#define GAME_HPP

#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "position.hpp"
#include "time.hpp"

const int GAME_TIMER_START = TICKS_PER_SECOND * 1000;  // 1000 secondi

class Game {
protected:
    bool running;
    int timer;
    int score;

    // Punteggio al momento dell'ingresso nel livello corrente.
    // Alla morte del giocatore il punteggio torna a questo valore
    // cosi' i punti fatti nel tentativo fallito non restano acquisiti
    // (il livello viene resettato).
    int level_score;

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool win();
    bool lose();

    // Ritorna true se il livello corrente è cambiato (o è stato rimosso):
    // in quel caso il Level& del chiamante non è piu' valido e le altre
    // collisioni vanno saltate fino al frame successivo.
    bool handle_level_change(Level& level);

    void handle_item_collection(Level& level);

    void handle_collisions();

    void handle_input();

    void update();

public:
    Game();

    void run();

    int get_score();
};

#endif