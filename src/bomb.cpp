#include "bomb.hpp"
#include "map.hpp"
#include "player.hpp"
#include "utils.hpp"

void Bomb::update_explosion(Map& map, Direction d, bool set) {
    Position target = p;

    for (int i = 0; i < range; i++) {
        target = next_position(target, d);

        if (map.get_cell(target) == UNBREAKABLE_WALL) {
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
    exploding = false;
    blinking = false;

    bomb_timer = BOMB_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
    blink_timer = BLINK_TIMER_START;
}

Position Bomb::get_position() {
    return p;
}

bool Bomb::is_active() {
    return active;
}

bool Bomb::is_exploding() {
    return exploding;
}

bool Bomb::is_blinking() {
    return blinking;
}

void Bomb::place(Map& map, Position _p, int _range) {
    map.set_cell(_p, BOMB);
    p = _p;
    range = _range;

    active = true;
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
}

void Bomb::update(Map& map) {
    if (!exploding) {
        if (blink_timer > 0) {
            blink_timer--;
        }
        if (bomb_timer > 0) {
            bomb_timer--;
        }

        if (blink_timer == 0) {
            blinking = !blinking;
            blink_timer = BLINK_TIMER_START;
        }

        if (bomb_timer == 0) {
            explode(map);
            map.set_cell(p, EMPTY);
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
    blinking = false;

    bomb_timer = BOMB_TIMER_START;
    explosion_timer = EXPLOSION_TIMER_START;
    blink_timer = BLINK_TIMER_START;
}