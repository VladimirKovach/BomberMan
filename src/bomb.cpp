#include "bomb.hpp"
#include "map.hpp"
#include "player.hpp"

Position Bomb::next_position(Position _p, Direction d) {
    Position next = _p;

    switch (d) {
        case UP:
            next.y--;
            break;

        case LEFT:
            next.x--;
            break;

        case DOWN:
            next.y++;
            break;

        case RIGHT:
            next.x++;
            break;

        default:
            break;
    }

    return next;
}

void Bomb::start_explosion(Map& map, Direction d) {
    Position target = p;

    for (int i = 0; i < range; i++) {
        target = next_position(target, d);

        if (map.get_cell(target) == UNBREAKABLE_WALL) {
            return;
        }
        else {
            map.set_explosion(target);
        }
    }
}

void Bomb::end_explosion(Map& map, Direction d) {
    Position target = p;

    for (int i = 0; i < range; i++) {
        target = next_position(target, d);

        if (map.get_cell(target) == UNBREAKABLE_WALL) {
            return;
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

    blink_timer = 10;         // 0.5 secondi
    explosion_timer = 60;     // 3.0 secondi
    explosion_duration = 30;  // 1.5 secondi
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


void Bomb::place(Position _p, int _range) {
    p = _p;
    range = _range;

    active = true;
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
}


void Bomb::update(Map& map) {
    if (!exploding) {
        if (blink_timer > 0) {
            blink_timer--;
        }
        if (explosion_timer > 0) {
            explosion_timer--;
        }
    }
    else if (exploding && explosion_duration > 0) {
        explosion_duration--;
    }

    if (!exploding) {
        if (blink_timer <= 0) {
            blinking = !blinking;
            blink_timer = 10;
        }
        if (explosion_timer <= 0) {
            explode(map);
            explosion_timer = 60;
        }
    }
    else if (exploding && explosion_duration <= 0) {
        map.unset_explosion(p);
        end_explosion(map, UP);
        end_explosion(map, LEFT);
        end_explosion(map, DOWN);
        end_explosion(map, RIGHT);

        explosion_duration = 30;
        reset();
    }
}

void Bomb::reset() {
    active = false;
    exploding = false;
    blinking = false;
}