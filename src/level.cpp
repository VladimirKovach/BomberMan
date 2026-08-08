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

Enemy* Level::get_enemies() {
    return enemies;
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

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].is_dead()) {
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
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].is_dead()) {
                enemies[i].update(map, player_p);
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
            enemies[i] = Enemy(p, 2, WALKER);
        }
    }
    else if (number == 2) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            enemies[i] = Enemy(p, 2, WALKER);
        }

        enemies[3] = Enemy(map.get_random_spawn(), 2, ROAMER);
    }
    else if (number == 3) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            enemies[i] = Enemy(p, 2, WALKER);
        }

        enemies[3] = Enemy(map.get_random_spawn(), 2, ROAMER);

        enemies[4] = Enemy(map.get_random_spawn(), 2, CHASER);
    }
    else if (number == 4) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            enemies[i] = Enemy(p, 2, WALKER);
        }

        enemies[3] = Enemy(map.get_random_spawn(), 2, ROAMER);
        enemies[4] = Enemy(map.get_random_spawn(), 2, ROAMER);

        enemies[5] = Enemy(map.get_random_spawn(), 2, CHASER);
    }
    else if (number == 5) {
        for (int i = 0; i < 3; i++) {
            Position p = map.get_random_spawn();
            enemies[i] = Enemy(p, 2, WALKER);
        }

        enemies[3] = Enemy(map.get_random_spawn(), 2, ROAMER);
        enemies[4] = Enemy(map.get_random_spawn(), 2, ROAMER);

        enemies[5] = Enemy(map.get_random_spawn(), 2, CHASER);
        enemies[6] = Enemy(map.get_random_spawn(), 2, CHASER);
    }
}

void Level::update(Position player_p) {
    update_bombs();
    update_enemies(player_p);
}

void Level::reset() {
    completed = false;

    map.reset();

    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].reset();
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].reset();
    }

    // Gli item a terra vanno rimossi: la griglia torna allo stato iniziale
    // e i muri ripristinati potrebbero coprire item rimasti a terra
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].reset();
    }
}