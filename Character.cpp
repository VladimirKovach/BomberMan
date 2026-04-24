#include "Character.hpp"

Character::Character(Position _p, int _lives, CellContent _type) {
    p = _p;
    lives = _lives;
    type = _type;
    under = EMPTY;
}

void Character::take_damage() {
    lives--;
}

bool Character::is_dead() {
    return lives <= 0;
}

Position Character::get_position() {
    return p;
}


Position Character::get_next_position(Direction d) {
    Position next_p = p;

    switch (d) {
        case UP:
            (next_p.y)--;
            break;

        case LEFT:
            (next_p.x)--;
            break;

        case DOWN:
            (next_p.y)++;
            break;

        case RIGHT:
            (next_p.x)++;
            break;

        default:  // NONE
            break;
    }

    return next_p;
}


CellContent Character::get_under() {
    return under;
}


void Character::move(Map& map, Direction d) {
    Position next_p = get_next_position(d);

    if (!map.is_wall(next_p)) {
        if (under == EMPTY) {
            map.clear_cell(p);
        }
        else {
            map.set_cell_content(p, under);
        }

        p = next_p;

        if (!map.is_explosion(p) && !map.is_enemy(p) && !map.is_door(p)) {
            under = map.get_cell_content(p);
            map.set_cell_content(p, type);
        }
    }
}