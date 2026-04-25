#include "Bomb.hpp"

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;
    place_time = -1.0;  // per convenzione
    exploded = false;
}

void Bomb::place(Position _p, double game_timer, int _range) {
    p = _p;
    place_time = game_timer;
    range = _range;
}

Position Bomb::get_position() {
    return p;
}

bool Bomb::is_exploded() {
    return exploded;
}

bool Bomb::is_timer_finished(double game_timer) {
    return place_time - game_timer >= TIMER;
}

void Bomb::explode(Map& map) {
    if (!exploded) {
        exploded = true;

        // Esplosione al centro
        //map.set_cell_content(p, EXPLOSION);
        map.set_explosion(p, EXPLOSION_DURATION);

        // Esplosione nelle 4 direzioni (croce)
        const int DIRECTIONS = 4;
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < DIRECTIONS; i++) {
            for (int j = 1; j <= range; j++) {
                Position target = {p.x + (dx[i] * j), p.y + (dy[i] * j)};

                if (map.get_cell_content(target) == UNBREAKABLE_WALL) {
                    break;
                }

                //map.set_cell_content(target, EXPLOSION);
                map.set_explosion(target, EXPLOSION_DURATION);
            }
        }
    }
}


void Bomb::reset() {
    exploded = false;
}