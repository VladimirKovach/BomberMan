#include "level.hpp"
#include "bomb.hpp"
#include "dummy_enemy.hpp"
#include "map.hpp"
#include "smart_enemy.hpp"

Level::Level(int _difficulty) {
    difficulty = _difficulty;
    completed = false;
    map = Map(_difficulty);
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

DummyEnemy* Level::get_dummy_enemies() {
    return dummy_enemies;
}

SmartEnemy* Level::get_smart_enemies() {
    return smart_enemies;
}
/*
Item* Level::get_items() {
    return items;
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
*/
int Level::get_bombs_count() {
    int count = 0;
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            count++;
        }
    }
    return count;
}

bool Level::all_enemies_dead() {
    if (completed) {
        return true;
    }
    else {
        int count = 0;
        for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
            if (!dummy_enemies[i].is_dead()) {
                count++;
            }
        }
        for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
            if (!smart_enemies[i].is_dead()) {
                count++;
            }
        }
        return count == 0;
    }
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
        for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
            if (!dummy_enemies[i].is_dead()) {
                dummy_enemies[i].update(map);
            }
        }

        for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
            if (!smart_enemies[i].is_dead()) {
                smart_enemies[i].update(map, player_p);
            }
        }

        if (all_enemies_dead()) {
            completed = true;
        }
    }
}


void Level::spawn_enemies() {
    for (int i = 0; i < difficulty; i++) {
        Position spawn_p = map.get_random_spawn();
        dummy_enemies[i] = DummyEnemy(spawn_p, 1);
    }

    for (int i = 0; i < difficulty; i++) {
        Position spawn_p = map.get_random_spawn();
        smart_enemies[i] = SmartEnemy(spawn_p, 2);
    }
}

void Level::reset() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].reset();
    }
    for (int i = 0; i < MAX_DUMMY_ENEMIES; i++) {
        dummy_enemies[i].reset();
    }
    for (int i = 0; i < MAX_SMART_ENEMIES; i++) {
        smart_enemies[i].reset();
    }
    /*
    // Gli item a terra vanno rimossi: la griglia torna allo stato iniziale
    // e i muri ripristinati potrebbero coprire item rimasti a terra
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].reset();
    }
    */
    map.reset();
}