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

    placement_time = -1.0;  // per convenzione
    explosion_time = -1.0;  // per convenzione
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


void Bomb::place(Position _p, int _range, double game_timer) {
    p = _p;
    range = _range;

    active = true;
    placement_time = game_timer;
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
}