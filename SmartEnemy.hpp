#include "DummyEnemy.hpp"

class SmartEnemy: DummyEnemy {
protected:
    double get_distance(Position p, Position q);

    Direction choose_direction();

public:
    SmartEnemy(Position _p, int _lives, int _speed);
};