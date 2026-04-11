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
    exploded = false;
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
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int d = 0; d < 4; d++) {
            for (int i = 1; i <= range; i++) {
                Position target = {p.x + (dx[d] * i), p.y + (dy[d] * i)};
                if (!map.cell_exists(target)) {
                    break;
                }

                CellContent content = map.get_cell_content(target);
                if (content == UNBREAKABLE_WALL) {
                    break;
                }

                map.set_explosion(target, EXPLOSION_DURATION);

                // Se colpisce un'altra bomba, la fa esplodere a catena (gestito dal game loop)
            }
        }
    }
}