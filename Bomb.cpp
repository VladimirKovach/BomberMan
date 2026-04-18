#include "Bomb.hpp"

Bomb::Bomb(Position _p, int _range, double _timer) {
    p = _p;
    range = _range;
    timer = _timer;
    place_timer = -1.0;  // per convenzione
    exploded = false;
}

void Bomb::place(Position _p, double _place_timer, int _range, double _timer) {
    p = _p;
    place_timer = _place_timer;
    range = _range;
    timer = _timer;
}

Position Bomb::get_position() {
    return p;
}

bool Bomb::is_exploded() {
    return exploded;
}

bool Bomb::is_timer_expired(double game_timer) {
    return (place_timer - game_timer) >= timer;
}

void Bomb::explode(Map& map) {
    if (!exploded) {
        exploded = true;

        // Esplosione al centro
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

                map.set_explosion(target, EXPLOSION_DURATION);
            }
        }
    }
}

void Bomb::reset() {
    exploded = false;
}