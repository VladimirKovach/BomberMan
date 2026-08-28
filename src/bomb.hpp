#ifndef BOMB_HPP
#define BOMB_HPP

#include "map.hpp"
#include "utils.hpp"
#include "item.hpp"

const int EXPLODING_TIMER_START = TICKS_PER_SECOND * 2;  // 2.00 secondi
const int EXPLOSION_TIMER_START = TICKS_PER_SECOND / 2;  // 0.50 secondi

// Il raggio base è 1.
// Con il buff diventa 1 + RANGE_BONUS (vedi Player::apply_buff). Attualmente vale 2.
const int MAX_EXPLOSION_RANGE = 1 + RANGE_BONUS;

// QUANTE CELLE puo' accendere al massimo una singola esplosione.
// Le celle a distanza esattamente d dal centro sono 4*d, quindi sommando
// tutti gli anelli da 0 a r: 1 + 4*(1 + 2 + ... + r) = 1 + 2*r*(r+1).
// Con r = 2 il risultato e' 13. Serve a dimensionare gli array a compile time,
// senza allocazione dinamica.
const int MAX_EXPLOSION_CELLS = 2 * MAX_EXPLOSION_RANGE * (MAX_EXPLOSION_RANGE + 1) + 1;

class Bomb {
protected:
    Position p;
    int range;

    // Celle accese da QUESTA esplosione, con il loro numero effettivo.
    // Vanno memorizzate perche' fra accensione e spegnimento i muri colpiti
    // vengono distrutti: ricalcolando la BFS sulla griglia ormai modificata
    // si otterrebbe un insieme diverso da quello acceso.

    // frame 40   ignite()      accende le celle e le registra in lit_cells[]
    // frame 41   game.cpp      distrugge i muri colpiti
    // frame 50   extinguish()  spegne le celle di lit_cells[]



    // lit_cells:  [{5,5}] [{4,5}] [{5,4}] [{6,5}] [ ?? ] [ ?? ] ... [ ?? ]
    //                0       1       2       3       4      5         12
    //                                                ↑
    //                                          lit_count = 4
    Position lit_cells[MAX_EXPLOSION_CELLS]; // numero effettivo
    int lit_count;

    bool active;
    bool exploding;

    int exploding_timer;
    int explosion_timer;

    // BFS a partire dalla bomba: accende ogni cella entro
    // 'range' passi, aggirando gli ostacoli. La forma risultante e' un rombo.
    void ignite(Map& map);

    // Spegne le celle registrate da ignite().
    void extinguish(Map& map);

public:
    Bomb(Position _p = {-1, -1}, int _range = 1);

    Position get_position();
    int get_range();

    bool is_active();
    bool is_exploding();

    void place(Map& map, Position _p, int _range);

    void explode(Map& map);

    void update(Map& map);

    void reset();
};

#endif