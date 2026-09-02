#include "level.hpp"
#include "position.hpp"
#include <cstdlib>

void Level::try_drop_item(Position p, int chance) {
    if (rand() % 100 < chance) {
        int r = rand() % 4;

        ItemType type = ITEM_RANGE;

        if (r == 1) {
            type = ITEM_LIFE;
        }
        else if (r == 2) {
            type = ITEM_SCORE;
        }
        else if (r == 3) {
            type = ITEM_TIME;
        }

        spawn_item(p, type);
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

void Level::spawn_enemies() {
    if (number == 1) {
        walkers[0] = Walker(map.get_random_spawn(), 1);
        walkers[1] = Walker(map.get_random_spawn(), 1);
        walkers[2] = Walker(map.get_random_spawn(), 1);
    }
    else if (number == 2) {
        walkers[0] = Walker(map.get_random_spawn(), 2);
        walkers[1] = Walker(map.get_random_spawn(), 2);
        walkers[2] = Walker(map.get_random_spawn(), 2);

        roamers[0] = Roamer(map.get_random_spawn(), 1);
    }
    else if (number == 3) {
        walkers[0] = Walker(map.get_random_spawn(), 2);
        walkers[1] = Walker(map.get_random_spawn(), 2);
        walkers[2] = Walker(map.get_random_spawn(), 2);

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);
    }
    else if (number == 4) {
        walkers[0] = Walker(map.get_random_spawn(), 2);
        walkers[1] = Walker(map.get_random_spawn(), 2);
        walkers[2] = Walker(map.get_random_spawn(), 2);

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);

        chasers[0] = Chaser(map.get_random_spawn(), 2);
    }
    else if (number == 5) {
        walkers[0] = Walker(map.get_random_spawn(), 2);
        walkers[1] = Walker(map.get_random_spawn(), 2);
        walkers[2] = Walker(map.get_random_spawn(), 2);

        roamers[0] = Roamer(map.get_random_spawn(), 2);
        roamers[1] = Roamer(map.get_random_spawn(), 2);

        chasers[0] = Chaser(map.get_random_spawn(), 2);
        chasers[1] = Chaser(map.get_random_spawn(), 2);
    }
}

Level::Level(int _number) {
    number = _number;
    completed = false;
    map = Map(number);

    spawn_enemies();
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

void Level::place_bomb(Position p, int range) {
    if (get_bomb_count() < MAX_BOMBS && !map.is_bomb(p)) {
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (!bombs[i].is_active()) {
                bombs[i].place(map, p, range);
                return;
            }
        }
    }
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

bool Level::player_enemies_collisions(Position player_p) {
    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead() && equal(player_p, chasers[i].get_position())) {
            return true;
        }
    }

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead() && equal(player_p, roamers[i].get_position())) {
            return true;
        }
    }

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead() && equal(player_p, walkers[i].get_position())) {
            return true;
        }
    }

    return false;
}

bool Level::player_explosions_collisions(Position player_p) {
    return map.is_explosion(player_p);
}

int Level::walls_explosions_collisions() {
    int breaks = 0;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Position p = {y, x};

            if (map.is_explosion(p) && map.is_wall_destructible(p)) {
                map.break_wall(p);
                breaks++;

                try_drop_item(p, WALL_DROP_CHANCE);
            }
        }
    }

    return breaks;
}

int Level::chasers_explosions_collisions() {
    int kills = 0;

    for (int i = 0; i < MAX_CHASERS; i++) {
        if (!chasers[i].is_dead()) {
            Position chaser_p = chasers[i].get_position();

            if (map.is_explosion(chaser_p)) {
                chasers[i].kill();
                kills++;

                try_drop_item(chaser_p, ENEMY_DROP_CHANCE);
            }
        }
    }

    return kills;
}

int Level::roamers_explosions_collisions() {
    int kills = 0;

    for (int i = 0; i < MAX_ROAMERS; i++) {
        if (!roamers[i].is_dead()) {
            Position roamer_p = roamers[i].get_position();

            if (map.is_explosion(roamer_p)) {
                roamers[i].kill();
                kills++;

                try_drop_item(roamer_p, ENEMY_DROP_CHANCE);
            }
        }
    }

    return kills;
}

int Level::walkers_explosions_collisions() {
    int kills = 0;

    for (int i = 0; i < MAX_WALKERS; i++) {
        if (!walkers[i].is_dead()) {
            Position walker_p = walkers[i].get_position();

            if (map.is_explosion(walker_p)) {
                walkers[i].kill();
                kills++;

                try_drop_item(walker_p, ENEMY_DROP_CHANCE);
            }
        }
    }

    return kills;
}

void Level::bombs_explosions_collisions() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].is_active()) {
            Position bomb_p = bombs[i].get_position();

            // Reazione a catena
            if (!bombs[i].is_exploding() && map.is_explosion(bomb_p)) {
                bombs[i].explode(map);
            }
        }
    }
}

void Level::update(Position player_p) {
    update_bombs();

    if (!completed) {
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