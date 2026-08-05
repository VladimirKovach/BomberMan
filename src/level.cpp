#include "level.hpp"
#include "bomb.hpp"
#include "map.hpp"
#include "utils.hpp"

Level::Level(int _number) {
    number = _number;
    completed = false;
    map = Map(number);
}

int Level::get_number() {
    return number;
}

bool Level::is_completed() {
    return completed;
}

Map& Level::get_map() {
    return map;
}

Bomb* Level::get_bombs() {
    return bombs;
}

Item* Level::get_items() {
    return items;
}

Roamer* Level::get_roamers() {
    return roamers;
}

Walker* Level::get_walkers() {
    return walkers;
}

bool Level::spawn_item(Position p, ItemType type) {
    // Cerco il primo slot libero
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].is_active()) {
            items[i].spawn(p, type);
            return true;
        }
    }
    return false;  // nessuno slot disponibile
}

int Level::get_bomb_count() {
    int count = 0;

    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            count++;
        }
    }

    return count;
}

int Level::get_enemy_count() {
    int count = 0;

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead()) {
            count++;
        }
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead()) {
            count++;
        }
    }

    return count;
}

void Level::update_bombs() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            bombs[i].update(map);
        }
    }
}


void Level::update_enemies(Position player_p) {
    if (!completed) {
        for (int i = 0; i < MAX_ROAMERS; i++) {
            if (!roamers[i].is_dead()) {
                roamers[i].update(map);
            }
        }

        for (int i = 0; i < MAX_WALKERS; i++) {
            if (!walkers[i].is_dead()) {
                walkers[i].update(map);
            }
        }

        if (get_enemy_count() == 0) {
            completed = true;
        }
    }
}


void Level::spawn_enemies() {
    if (number == 1) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            walkers[i] = Walker(p, 2);
        }
    }
    else if (number == 2) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            roamers[i] = Roamer(p, 1);
        }
    }
    else {
        for (int i = 0; i < number; i++) {
            Position spawn_p = map.get_random_spawn();
            walkers[i] = Walker(spawn_p, 1);
        }

        for (int i = 0; i < number; i++) {
            Position spawn_p = map.get_random_spawn();
            roamers[i] = Roamer(spawn_p, 2);
        }
    }
}

void Level::reset() {
    completed = false;

    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].reset();
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        walkers[i].reset();
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        roamers[i].reset();
    }

    // Gli item a terra vanno rimossi: la griglia torna allo stato iniziale
    // e i muri ripristinati potrebbero coprire item rimasti a terra
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].reset();
    }

    map.reset();
}