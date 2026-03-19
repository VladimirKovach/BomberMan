#include "Character.hpp"

Character::Character(Position _pos, int _lives, int _speed) {
    pos = _pos;
    lives = _lives;
    speed = _speed;  // da implementare
}

bool Character::is_dead() {
    return lives <= 0;
}

Position Character::get_position() {
    return pos;
}


Position Character::get_next_position(Direction dir) {
    Position next_pos = pos;

    switch (dir) {
        case UP:
            (next_pos.y)--;
            break;

        case LEFT:
            (next_pos.x)--;
            break;

        case DOWN:
            (next_pos.y)++;
            break;

        case RIGHT:
            (next_pos.x)++;
            break;

        default:  // NONE
            break;
    }

    return next_pos;
}


void Character::move(Map& map, Direction dir) {
    Position next_pos = get_next_position(dir);

    if (map.is_walkable_cell(next_pos)) {
        CellContent content = map.get_cell_content(pos);
        map.clear_cell(pos);
        pos = next_pos;
        map.set_cell_content(pos, content);
    }
}