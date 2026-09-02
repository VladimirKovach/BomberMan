#include "level_manager.hpp"
#include <cstdlib>

LevelManager::LevelManager() {
    head = NULL;
    current = NULL;

    Node* prev_node = NULL;

    for (int i = 1; i <= NUM_LEVELS; i++) {
        Node* node = new Node;

        node->level = Level(i);
        node->next = NULL;
        node->prev = prev_node;

        if (prev_node != NULL) {
            prev_node->next = node;
        }
        else {
            head = node;
        }

        prev_node = node;
    }

    current = head;
}

LevelManager::~LevelManager() {
    Node* node = head;
    while (node != NULL) {
        Node* next = node->next;
        delete node;
        node = next;
    }
    head = NULL;
    current = NULL;
}

Level& LevelManager::get_current_level() {
    return current->level;
}

bool LevelManager::has_next_level() {
    return (current != NULL && current->next != NULL);
}

bool LevelManager::has_prev_level() {
    return (current != NULL && current->prev != NULL);
}

void LevelManager::go_to_next_level() {
    if (current != NULL && current->next != NULL) {
        current = current->next;
    }
}

void LevelManager::go_to_prev_level() {
    if (current != NULL && current->prev != NULL) {
        current = current->prev;
    }
}

bool LevelManager::all_levels_completed() {
    Node* node = head;
    while (node != NULL) {
        if (!(node->level).is_completed()) {
            return false;
        }
        node = node->next;
    }
    return (head != NULL);
}

void LevelManager::update_doors() {
    if (current != NULL) {
        Map& map = (current->level).get_map();

        if (current->next != NULL) {
            map.open_door_next();
        }
        else {
            map.close_door_next();
        }

        if (current->prev != NULL) {
            map.open_door_prev();
        }
        else {
            map.close_door_prev();
        }
    }
}

void LevelManager::free_levels() {
    Node* node = head;
    while (node != NULL) {
        Node* next = node->next;
        delete node;
        node = next;
    }
    head = NULL;
    current = NULL;
}


bool LevelManager::remove_current_level(bool forward) {
    if (current == NULL) {
        return false;
    }

    Node* to_remove = current;
    Node* new_current;

    if (forward) {
        new_current = to_remove->next;
    }
    else {
        new_current = to_remove->prev;
    }

    if (new_current == NULL) {
        return false;
    }

    Node* prev_node = to_remove->prev;
    Node* next_node = to_remove->next;

    if (prev_node != NULL) {
        prev_node->next = next_node;
    }
    if (next_node != NULL) {
        next_node->prev = prev_node;
    }

    if (head == to_remove) {
        head = next_node;
    }

    current = new_current;
    delete to_remove;

    return true;
}