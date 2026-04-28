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
}


void Bomb::explode(Map& map, double game_timer) {
    exploded = true;
    explosion_time = game_timer;

    // Esplosione al centro
    map.set_cell_content(p, EXPLOSION);

    // Esplosione nelle 4 direzioni
    for (int i = 1; i <= range; i++) {
        Position target_up = {p.x, p.y - i};
        if (map.get_cell_content(target_up) == UNBREAKABLE_WALL) {
            break;
        }
        else {
            map.set_cell_content(target_up, EXPLOSION);
        }
    }

    for (int i = 1; i <= range; i++) {
        Position target_left = {p.x - i, p.y};
        if (map.get_cell_content(target_left) == UNBREAKABLE_WALL) {
            break;
        }
        else {
            map.set_cell_content(target_left, EXPLOSION);
        }
    }

    for (int i = 1; i <= range; i++) {
        Position target_down = {p.x, p.y + i};
        if (map.get_cell_content(target_down) == UNBREAKABLE_WALL) {
            break;
        }
        else {
            map.set_cell_content(target_down, EXPLOSION);
        }
    }

    for (int i = 1; i <= range; i++) {
        Position target_right = {p.x + i, p.y};
        if (map.get_cell_content(target_right) == UNBREAKABLE_WALL) {
            break;
        }
        else {
            map.set_cell_content(target_right, EXPLOSION);
        }
    }
}


void Bomb::update(Map& map, double game_timer) {
    if (!exploded && placement_time - game_timer >= EXPLOSION_TIMER) {
        explode(map, game_timer);
    }

    else if (exploded && explosion_time - game_timer >= EXPLOSION_DURATION) {
        map.clear_cell(p);

        for (int i = 1; i <= range; i++) {
            Position target_up = {p.x, p.y - i};
            if (map.get_cell_content(target_up) == EXPLOSION) {
                map.clear_cell(target_up);
            }
            else {
                break;
            }
        }

        for (int i = 1; i <= range; i++) {
            Position target_left = {p.x - i, p.y};
            if (map.get_cell_content(target_left) == EXPLOSION) {
                map.clear_cell(target_left);
            }
            else {
                break;
            }
        }

        for (int i = 1; i <= range; i++) {
            Position target_down = {p.x, p.y + i};
            if (map.get_cell_content(target_down) == EXPLOSION) {
                map.clear_cell(target_down);
            }
            else {
                break;
            }
        }

        for (int i = 1; i <= range; i++) {
            Position target_right = {p.x + i, p.y};
            if (map.get_cell_content(target_right) == EXPLOSION) {
                map.clear_cell(target_right);
            }
            else {
                break;
            }
        }

        active = false;
        exploded = false;
    }
}