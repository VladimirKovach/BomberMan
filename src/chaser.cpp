#include "chaser.hpp"
#include <cstdlib>

void Chaser::move(Map& map) {

}

Chaser::Chaser(Position p, int speed) : Enemy(p, speed) {}

void Chaser::update(Map& map, Position player_p) {
    if (_move_timer > 0) {
        _move_timer--;
    }

    if (_move_timer == 0) {
        move(map);

        

        _move_timer = TICKS_PER_SECOND / _speed;
    }
}