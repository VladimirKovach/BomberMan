#include "Bomb.hpp"

Bomb::Bomb() {
    p = {0, 0};
    range = 1;
    timer = BOMB_DEFAULT_TIMER;
    active = false;
    exploded = false;
}

Bomb::Bomb(Position _p, int _range, int _timer) {
    p = _p;
    range = _range;
    timer = _timer;
    active = true;
    exploded = false;
}

void Bomb::place(Position _p, int _range, int _timer) {
    p = _p;
    range = _range;
    timer = _timer;
    active = true;
    exploded = false;
}

Position Bomb::get_position() {
    return p;
}

int Bomb::get_range() {
    return range;
}

bool Bomb::is_active() {
    return active;
}

bool Bomb::has_exploded() {
    return exploded;
}

bool Bomb::is_timer_expired() {
    return active && timer <= 0;
}

void Bomb::update_timer() {
    if (active && !exploded) {
        timer--;
    }
}

void Bomb::explode(Map& map) {
    if (!active || exploded) return;

    exploded = true;
    active = false;

    // Esplosione al centro
    map.set_explosion(p, EXPLOSION_DURATION);

    // Esplosione nelle 4 direzioni (croce)
    // dx/dy: le 4 direzioni cardinali
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        for (int i = 1; i <= range; i++) {
            Position target = {p.x + dx[dir] * i, p.y + dy[dir] * i};

            if (!map.cell_exists(target)) break;

            CellContent content = map.get_cell_content(target);

            if (content == UNBREAKABLE_WALL) {
                break;
            }

            if (content == BREAKABLE_WALL) {
                map.set_explosion(target, EXPLOSION_DURATION);
                break;
            }

            // EMPTY, PLAYER, ENEMY, ITEM, BOMB — l'esplosione passa
            map.set_explosion(target, EXPLOSION_DURATION);

            // Se colpisce un'altra bomba, la farà esplodere a catena
            // (gestito dal Game loop)
        }
    }
}

void Bomb::reset() {
    active = false;
    exploded = false;
    timer = BOMB_DEFAULT_TIMER;
}