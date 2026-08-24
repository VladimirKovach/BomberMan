#include "level.hpp"
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

Chaser* Level::get_chasers() {
    return chasers;
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

void Level::spawn_enemies() {
    if (number == 1) {
        for (int i = 0; i < 3; i++) {
            walkers[i] = Walker(map.get_random_spawn(), 1);
        }
    }
    else if (number == 2) {
        for (int i = 0; i < 3; i++) {
            walkers[i] = Walker(map.get_random_spawn(), 2);
        }

        roamers[0] = Roamer(map.get_random_spawn(), 1);
    }
    else if (number == 3) {
        for (int i = 0; i < 3; i++) {
            walkers[i] = Walker(map.get_random_spawn(), 2);
        }

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);
    }
    else if (number == 4) {
        for (int i = 0; i < 3; i++) {
            walkers[i] = Walker(map.get_random_spawn(), 2);
        }

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);

        chasers[0] = Chaser(map.get_random_spawn(), 2);
    }
    else if (number == 5) {
        for (int i = 0; i < 3; i++) {
            walkers[i] = Walker(map.get_random_spawn(), 2);
        }

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);

        chasers[0] = Chaser(map.get_random_spawn(), 2);
        chasers[1] = Chaser(map.get_random_spawn(), 2);
    }
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

    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            count++;
        }
    }

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
    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            chasers[i].update(map, player_p);
        }
    }

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
}

void Level::update(Position player_p) {
    if (!completed) {
        update_bombs();
        update_enemies(player_p);
    }

    if (get_enemy_count() == 0) {
        completed = true;
    }
}

void Level::reset() {
    completed = false;

    map.reset();

    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].reset();
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].reset();
    }

    for (int i = 0; i < MAX_CHASERS; i++) {
        chasers[i].reset();
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        roamers[i].reset();
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        walkers[i].reset();
    }
}