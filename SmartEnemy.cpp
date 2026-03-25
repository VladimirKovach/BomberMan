#include "SmartEnemy.hpp"
#include <cmath>

SmartEnemy::SmartEnemy(Position _p, int _lives, int _speed) : DummyEnemy(_p, _lives, _speed) {}

double SmartEnemy::get_distance(Position p1, Position p2) {
    int dx = (p1.x - p2.x);
    int dy = (p1.y - p2.y);
    return sqrt((dx * dx) + (dy * dy));
}

Direction SmartEnemy::choose_direction() {

}