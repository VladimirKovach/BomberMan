#include "bomb.hpp"
#include "map.hpp"
#include "player.hpp"
#include "utils.hpp"

void Bomb::update_explosion(Map& map, Direction d, bool set) {
    Position target = _p;

    for (int i = 0; i < _range; i++) {
        target = next_position(target, d);

        if (map.out_of_bounds(target) || map.get_cell(target) == UNBREAKABLE_WALL) {
            return;
        }

        if (set) {
            map.set_explosion(target);
        }
        else {
            map.unset_explosion(target);
        }
    }
}

Bomb::Bomb(Position p, int range) {
    _p = p;
    _range = range;

    _active = false;
    _blinking = false;
    _exploding = false;

    _blinking_timer = BLINKING_TIMER_START;
    _exploding_timer = EXPLODING_TIMER_START;
    _explosion_timer = EXPLOSION_TIMER_START;
}

Position Bomb::get_position() {
    return _p;
}

int Bomb::get_range() {
    return _range;
}

bool Bomb::is_active() {
    return _active;
}

bool Bomb::is_blinking() {
    return _blinking;
}

bool Bomb::is_exploding() {
    return _exploding;
}

void Bomb::place(Map& map, Position p, int range) {
    _p = p;
    _range = range;

    _active = true;

    map.set_cell(_p, BOMB);
}

void Bomb::explode(Map& map) {
    _exploding = true;

    // Esplosione al centro
    map.set_explosion(_p);

    // Esplosione nelle 4 direzioni (croce)
    update_explosion(map, UP, true);
    update_explosion(map, LEFT, true);
    update_explosion(map, DOWN, true);
    update_explosion(map, RIGHT, true);

    map.set_cell(_p, EMPTY);
}

void Bomb::update(Map& map) {
    if (!_exploding) {
        if (_blinking_timer > 0) {
            _blinking_timer--;
        }
        if (_exploding_timer > 0) {
            _exploding_timer--;
        }

        if (_blinking_timer == 0) {
            _blinking = !_blinking;
            _blinking_timer = BLINKING_TIMER_START;
        }

        if (_exploding_timer == 0) {
            explode(map);
        }
    }
    else {
        if (_explosion_timer > 0) {
            _explosion_timer--;
        }

        if (_explosion_timer == 0) {
            map.unset_explosion(_p);

            update_explosion(map, UP, false);
            update_explosion(map, LEFT, false);
            update_explosion(map, DOWN, false);
            update_explosion(map, RIGHT, false);

            reset();
        }
    }
}

void Bomb::reset() {
    _active = false;
    _blinking = false;
    _exploding = false;

    _blinking_timer = BLINKING_TIMER_START;
    _exploding_timer = EXPLODING_TIMER_START;
    _explosion_timer = EXPLOSION_TIMER_START;
}