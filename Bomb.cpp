#include "Bomb.hpp"

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

    reset();

    // per convenzione
    placement_time = -1.0;
    explosion_time = -1.0;
    blink_state_start = -1.0;
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
    return blink_state;
}


void Bomb::place(Position _p, int _range, double game_timer) {
    p = _p;
    range = _range;

    active = true;
    blink_state = true;

    placement_time = game_timer;
    blink_state_start = game_timer;
}


void Bomb::explode(Map& map, double game_timer) {
    exploding = true;
    explosion_time = game_timer;

    // Esplosione al centro
    map.set_explosion(p);

    // Esplosione nelle 4 direzioni (croce)
    start_explosion(map, UP);
    start_explosion(map, LEFT);
    start_explosion(map, DOWN);
    start_explosion(map, RIGHT);
}


void Bomb::update(Map& map, double game_timer) {
    if (!exploding && placement_time - game_timer >= EXPLOSION_TIMER) {
        explode(map, game_timer);
    }
    else if (!exploding && blink_state_start - game_timer >= BLINK_DELTA) {
        blink_state_start = game_timer;
        blink_state = !blink_state;
    }
    else if (exploding && explosion_time - game_timer >= EXPLOSION_DURATION) {
        map.unset_explosion(p);
        end_explosion(map, UP);
        end_explosion(map, LEFT);
        end_explosion(map, DOWN);
        end_explosion(map, RIGHT);

        reset();
    }
}

void Bomb::reset() {
    active = false;
    exploding = false;
    blink_state = false;
}