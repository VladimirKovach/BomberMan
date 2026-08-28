#include "bomb.hpp"
#include "map.hpp"
#include "utils.hpp"

void Bomb::ignite(Map& map) {
    bool visited[MAP_HEIGHT][MAP_WIDTH];

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            visited[y][x] = false;
        }
    }

    Position queue[MAX_EXPLOSION_CELLS];
    int distance[MAX_EXPLOSION_CELLS];
    int head = 0;
    int tail = 0;

    const Direction dirs[DIRECTIONS_COUNT] = {UP, LEFT, DOWN, RIGHT};

    lit_count = 0;

    // La visita parte dalla cella della bomba, a distanza 0.
    queue[tail] = p;
    distance[tail] = 0;
    tail++;
    visited[p.y][p.x] = true;

    while (head < tail) {
        Position current = queue[head];
        int d = distance[head];
        head++;

        // Accendo la cella e la registro, per poterla spegnere dopo.
        map.set_explosion(current);
        lit_cells[lit_count] = current;
        lit_count++;

        // Un muro distruttibile viene colpito, ma assorbe l'esplosione:
        // le celle oltre non si accendono.
        if (map.is_wall_destructible(current)) {
            continue;
        }

        // Raggiunta la distanza massima: non espando oltre.
        if (d == range) {
            continue;
        }

        for (int i = 0; i < DIRECTIONS_COUNT; i++) {
            Position next = next_position(current, dirs[i]);

            // L'ordine dei controlli conta: out_of_bounds va verificato
            // prima di leggere visited[next.y][next.x].
            if (map.out_of_bounds(next)) {
                continue;
            }
            if (visited[next.y][next.x]) {
                continue;
            }
            if (map.is_wall_solid(next)) {
                continue;
            }
            if (tail >= MAX_EXPLOSION_CELLS) {
                continue;
            }

            visited[next.y][next.x] = true;
            queue[tail] = next;
            distance[tail] = d + 1;
            tail++;
        }
    }
}

void Bomb::extinguish(Map& map) {
    for (int i = 0; i < lit_count; i++) {
        map.unset_explosion(lit_cells[i]);
    }

    lit_count = 0;
}

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;

    lit_count = 0;

    active = false;
    exploding = false;

    exploding_timer = EXPLODING_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
}

Position Bomb::get_position() {
    return p;
}

int Bomb::get_range() {
    return range;
}

bool Bomb::is_active() {
    return active;
}

bool Bomb::is_exploding() {
    return exploding;
}

void Bomb::place(Map& map, Position _p, int _range) {
    p = _p;
    range = _range;
    active = true;

    map.set_bomb(p);
}

void Bomb::explode(Map& map) {
    // Guardia contro una doppia accensione.
    // ignite() azzera lit_count, quindi le celle della prima accensione non verrebbero mai spente.
    if (exploding) {
        return;
    }

    exploding = true;

    ignite(map);

    map.unset_bomb(p);
}

void Bomb::update(Map& map) {
    if (!exploding) {
        if (exploding_timer > 0) {
            exploding_timer--;
        }

        if (exploding_timer == 0) {
            explode(map);
        }
    }
    else {
        if (explosion_timer > 0) {
            explosion_timer--;
        }

        if (explosion_timer == 0) {
            extinguish(map);
            reset();
        }
    }
}

void Bomb::reset() {
    active = false;
    exploding = false;
    lit_count = 0;

    exploding_timer = EXPLODING_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
}