#include "Map.hpp"
#include <iostream>

Map::Map() {
    head = NULL;
    current = NULL;

    Node* prev_node = NULL;

    for (int i = 1; i <= NUM_LEVELS; i++) {
        Node* node = new Node;

        node->level = Level(i);
        node->next = NULL;
        node->prev = prev_node;

        (node->level).spawn_enemies();

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

Map::~Map() {
    Node* node = head;
    while (node != NULL) {
        Node* next = node->next;
        delete node;
        node = next;
    }
    head = NULL;
    current = NULL;
}

Level& Map::get_current_level() {
    return current->level;
}

bool Map::has_next_level() {
    return (current != NULL && current->next != NULL);
}

bool Map::has_prev_level() {
    return (current != NULL && current->prev != NULL);
}

void Map::go_to_next_level() {
    if (current != NULL && current->next != NULL) {
        current = current->next;
    }
}

void Map::go_to_prev_level() {
    if (current != NULL && current->prev != NULL) {
        current = current->prev;
    }
}

bool Map::is_current_completed() {
    if (current != NULL) {
        return (current->level).is_completed();
    }
    return false;
}

bool Map::all_levels_completed() {
    Node* node = head;
    while (node != NULL) {
        if (!(node->level).is_completed()) {
            return false;
        }
        node = node->next;
    }
    return (head != NULL);
}


void Map::update_all_bombs(double game_timer) {
    Node* tmp = head;
    while (tmp != NULL) {
        (tmp->level).update_bombs(game_timer);
        tmp = tmp->next;
    }
}

void Map::update_doors() {
    if (current != NULL) {
        Grid& grid = (current->level).get_grid();

        if (current->next != NULL) {
            grid.open_exit();
        }
        else {
            grid.close_exit();
        }

        if (current->prev != NULL) {
            grid.open_entrance();
        }
        else {
            grid.close_entrance();
        }
    }
}


bool Map::remove_current_level(bool forward) {
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