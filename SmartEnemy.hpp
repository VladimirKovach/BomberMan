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

    double get_distance(Position p1, Position p2);

    void sort_pairs();

    void update_player_position(Position _player_p);

    void plan_move();

public:
    SmartEnemy(Position _p = {-1, -1}, int _lives = 1, int _speed = 1);

    void update(Map& map, double game_timer, Position _player_p);
};