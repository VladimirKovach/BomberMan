#include "bomb.hpp"
#include "map.hpp"
#include "utils.hpp"

void Bomb::update_explosion(Map& map, Direction d, bool set) {
    Position target = p;

    for (int i = 0; i < range; i++) {
        target = next_position(target, d);

        if (map.out_of_bounds(target) || map.get_cell(target) == UNBREAKABLE_WALL) {
            return;
        }

        if (set) {
            map.set_explosion(target);
        }
        else {
            map.unset_explosion(target);
        }
    }
}

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;

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

    active = true;

    map.set_cell(p, BOMB);
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

    map.set_cell(p, EMPTY);
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
    blink = false;
    exploding = false;

    blink_timer = BLINK_TIMER_START;
    exploding_timer = EXPLODING_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
}