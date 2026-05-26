#include "Bomb.hpp"

void Bomb::begin_explosion(Grid& grid, Direction d) {
    for (int i = 0; i < range; i++) {
        Position target = get_next_position(p, d);
        if (grid.get_cell(target) == UNBREAKABLE_WALL) {
            break;
        }
        else {
            grid.set_explosion(target);
        }
    }
}

void Bomb::end_explosion(Grid& grid, Direction d) {
    for (int i = 0; i < range; i++) {
        Position target = get_next_position(p, d);
        if (grid.is_explosion(target)) {
            grid.unset_explosion(target);
        }
        else {
            break;
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


void Bomb::explode(Grid& grid, double game_timer) {
    exploding = true;
    explosion_time = game_timer;

    // Esplosione al centro
    grid.set_explosion(p);

    // Esplosione nelle 4 direzioni (croce)
    begin_explosion(grid, UP);
    begin_explosion(grid, LEFT);
    begin_explosion(grid, DOWN);
    begin_explosion(grid, RIGHT);
}


void Bomb::update(Grid& grid, double game_timer) {
    if (!exploding && placement_time - game_timer >= EXPLOSION_TIMER) {
        explode(grid, game_timer);
    }

    else if (!exploding && blink_state_start - game_timer >= BLINK_DELTA) {
        blink_state_start = game_timer;
        blink_state = !blink_state;
    }

    else if (exploding && explosion_time - game_timer >= EXPLOSION_DURATION) {
        grid.unset_explosion(p);
        end_explosion(grid, UP);
        end_explosion(grid, LEFT);
        end_explosion(grid, DOWN);
        end_explosion(grid, RIGHT);

        reset();
    }
}


void Bomb::reset() {
    active = false;
    exploding = false;
    blink_state = false;
}