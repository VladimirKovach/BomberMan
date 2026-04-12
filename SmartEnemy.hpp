#include "DummyEnemy.hpp"
#include "Character.hpp"

struct Pair {
    Direction direction;
    int distance;
};

class SmartEnemy: public DummyEnemy {
protected:
    Position player_p;
    Pair pairs[DIRECTION_COUNT];

    void init_pairs();

    void sort_pairs();

    double get_distance(Position p1, Position p2);

public:
    SmartEnemy(Position _p = {-1, -1}, int _lives = 1, int _speed = 2);

    void update_player_position(Position _player_p);

    void plan_movement();
};