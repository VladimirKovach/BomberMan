#include "bomb.hpp"
#include "map.hpp"
#include "utils.hpp"

void Bomb::update_explosion(Map& map, Direction d, bool set) {
    Position target = p;

    if (set) {
        // Accensione: propago fino al raggio, fermandomi sul primo muro solido.
        // Un muro distruttibile viene colpito ma assorbe l'esplosione, che
        // quindi non prosegue oltre.
        int reach = 0;

        for (int i = 0; i < range; i++) {
            target = next_position(target, d);

            if (map.out_of_bounds(target) || map.is_wall_solid(target)) {
                break;
            }

            map.set_explosion(target);
            reach++;

            if (map.is_wall_destructible(target)) {
                break;
            }
        }

        blast[d] = reach;
    }
    else {
        // Spegnimento: uso il raggio memorizzato. Non posso ricalcolarlo,
        // perche' i muri colpiti nel frattempo sono stati distrutti e la
        // griglia non li mostra piu'.
        for (int i = 0; i < blast[d]; i++) {
            target = next_position(target, d);
            map.unset_explosion(target);
        }

        blast[d] = 0;
    }
}

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        blast[i] = 0;
    }
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
    exploding = true;

    // Esplosione al centro
    map.set_explosion(p);

    // Esplosione nelle 4 direzioni (croce)
    update_explosion(map, UP, true);
    update_explosion(map, LEFT, true);
    update_explosion(map, DOWN, true);
    update_explosion(map, RIGHT, true);

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
            map.unset_explosion(p);

            update_explosion(map, UP, false);
            update_explosion(map, LEFT, false);
            update_explosion(map, DOWN, false);
            update_explosion(map, RIGHT, false);

            reset();
        }
    }
}

void Bomb::reset() {
    active = false;
    exploding = false;

    exploding_timer = EXPLODING_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
}