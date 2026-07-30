#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "map.hpp"
//#include "Item.hpp"

enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

const int MAX_LIVES = 3;  // aumentare a 5?

class Player {
protected:
    Position p;
    Position start_p;

    int lives;

    int bomb_range;    // raggio corrente delle bombe (1 = base)
    //double buff_end;   // valore dell'orologio di gioco a cui scade il buff raggio

    Position next_position(Direction d);

public:
    Player(Position _p = {1, 1}, int _lives = 3);

    Position get_position();
    void set_position(Position _p);

    int get_lives();
    void gain_life();
    void lose_life();
    bool is_dead();

    void move(Map& map, Direction d);

    int get_bomb_range();

    // Applica/estende il buff raggio. L'orologio di gioco game_clock e' decrescente.
    //void apply_range_buff(double duration, double game_clock);

    // Disattiva il buff se scaduto. Da chiamare ad ogni frame.
    //void update_buff(double game_clock);

    // Secondi rimanenti del buff raggio (0 se non attivo). Per l'HUD.
    //double get_buff_remaining(double game_clock);

    void reset();
};

#endif