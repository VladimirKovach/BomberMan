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
    // Quando il giocatore perde una vita il punteggio torna a questo valore,
    // cosi' i punti fatti nel tentativo fallito non restano acquisiti.
    // (il livello viene resettato).
    int level_score;

    // Quando il giocatore perde una vita, viene impostato a true,
    // cosi' da effettuare il reset dopo il disegno,
    // per poter visualizzare la collisione con un'esplosione o un nemico
    // (il disegno avviene dopo l'aggiornamento)
    bool reset;

    LevelManager level_manager;
    Player player;
    Renderer renderer;

    bool win();
    bool lose();

    // Restituisce true se il livello corrente e' cambiato (o è stato rimosso):
    // in quel caso il livello del chiamante non e' piu' valido
    // e le altre collisioni vanno saltate fino al frame successivo.
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