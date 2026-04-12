#include "Character.hpp"

Character::Character(Position _p, int _lives, CellContent _type) {
    p = _p;
    lives = _lives;
    type = _type;
    cell_under = EMPTY;
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


CellContent Character::get_cell_under() {
    return cell_under;
}

void Character::set_cell_under(CellContent content) {
    cell_under = content;
}


void Character::move(Map& map, Direction d) {
    Position next_p = get_next_position(d);

    if (map.is_walkable_cell(next_p)) {
        map.clear_cell(p);
        map.set_cell_content(p, cell_under);

        cell_under = map.get_cell_content(next_p);

        p = next_p;
        map.set_cell_content(p, type);
    }
}