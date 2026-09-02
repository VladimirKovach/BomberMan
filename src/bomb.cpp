#include "bomb.hpp"
#include "map.hpp"
#include "position.hpp"

// Propaga l'esplosione fino al raggio
// I muri solidi bloccano l'esplosione
// I muri distruttibili vengono distrutti e assorbono l'esplosione
void Bomb::start_explosion(Map& map, Direction d) {
    int effective_range = 0;
    Position target = p;

    for (int i = 0; i < range; i++) {
        target = next_position(target, d);

        if (map.out_of_bounds(target) || map.is_wall_solid(target)) {
            break;
        }

        map.set_explosion(target);
        effective_range++;

        if (map.is_wall_destructible(target)) {
            break;
        }
    }

    explosion_range[d] = effective_range;
}

void Bomb::stop_explosion(Map& map, Direction d) {
    Position target = p;

    for (int i = 0; i < explosion_range[d]; i++) {
        target = next_position(target, d);
        map.unset_explosion(target);
    }
}

Bomb::Bomb() {
    p = POSITION_NONE;
    range = 1;

    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        explosion_range[i] = 0;
    }

    active = false;
    blink = false;
    exploding = false;

    blink_timer = BLINK_TIMER_START;
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

bool Bomb::is_blinking() {
    return blink;
}

bool Bomb::is_exploding() {
    return exploding;
}

void Bomb::place(Map& map, Position _p, int _range) {
    p = _p;
    range = _range;

    if (range <= 0) {
        range = 1;
    }

    active = true;

    map.set_bomb(p);
}

void Bomb::explode(Map& map) {
    exploding = true;

    // Esplosione al centro
    map.set_explosion(p);

    // Esplosione nelle 4 direzioni (croce)
    start_explosion(map, UP);
    start_explosion(map, LEFT);
    start_explosion(map, DOWN);
    start_explosion(map, RIGHT);

    map.unset_bomb(p);
}

void Bomb::update(Map& map) {
    if (!exploding) {
        if (blink_timer > 0) {
            blink_timer--;
        }

        if (exploding_timer > 0) {
            exploding_timer--;
        }

        if (blink_timer == 0) {
            blink = !blink;
            blink_timer = BLINK_TIMER_START;
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

            stop_explosion(map, UP);
            stop_explosion(map, LEFT);
            stop_explosion(map, DOWN);
            stop_explosion(map, RIGHT);

            reset();
        }
    }
}

void Bomb::reset() {
    for (int i = 0; i < DIRECTIONS_COUNT; i++) {
        explosion_range[i] = 0;
    }

    active = false;
    blink = false;
    exploding = false;

    blink_timer = BLINK_TIMER_START;
    exploding_timer = EXPLODING_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
}