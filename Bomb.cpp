#include "Bomb.hpp"

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;

    active = false;
    exploded = false;

    placement_time = -1.0;  // per convenzione
    explosion_time = -1.0;  // per convenzione
}


Position Bomb::get_position() {
    return p;
}

bool Bomb::is_active() {
    return active;
}

bool Bomb::is_exploded() {
    return exploded;
}


void Bomb::place(Position _p, int _range, double game_timer) {
    p = _p;
    range = _range;

    active = true;
    placement_time = game_timer;

    targets[0] = {p.x, p.y};
    targets[1] = {p.x, p.y - 1};
    targets[2] = {p.x - 1, p.y};
    targets[3] = {p.x, p.y + 1};
    targets[4] = {p.x + 1, p.y};
}


void Bomb::explode(Map& map, double game_timer) {
    exploded = true;
    explosion_time = game_timer;

    for (int i = 0; i < TARGET_COUNT; i++) {
        if (map.get_cell_content(targets[i]) != UNBREAKABLE_WALL) {
            map.set_cell_content(targets[i], EXPLOSION);
        }
    }
}


void Bomb::update(Map& map, double game_timer) {
    if (!exploded && placement_time - game_timer >= EXPLOSION_TIMER) {
        explode(map, game_timer);
    }

    else if (exploded && explosion_time - game_timer >= EXPLOSION_DURATION) {
        for (int i = 0; i < TARGET_COUNT; i++) {
            if (map.is_explosion(targets[i])) {
                map.clear_cell(targets[i]);
            }
        }

        reset();
    }
}


void Bomb::reset() {
    active = false;
    exploded = false;
}