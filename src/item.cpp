#include "item.hpp"
#include "map.hpp"
#include <cstdlib>

Item::Item(Position _p, ItemType _type) {
    p = _p;
    type = _type;
    duration = 0.0;
    reset();
}

Position Item::get_position() {
    return p;
}

ItemType Item::get_type() {
    return type;
}

double Item::get_duration() {
    return duration;
}

bool Item::is_active() {
    return active;
}

void Item::spawn(Position _p, ItemType _type) {
    p = _p;
    type = _type;
    active = true;

    // Solo l'item raggio ha una durata: sorteggio tra corta e lunga
    if (type == ITEM_RANGE) {
        if (rand() % 2 == 0) {
            duration = RANGE_DURATION_SHORT;
        }
        else {
            duration = RANGE_DURATION_LONG;
        }
    }
    else {
        duration = 0.0;
    }
}

void Item::collect() {
    active = false;
}

void Item::reset() {
    active = false;
}