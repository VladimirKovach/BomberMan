#include "Bomb.hpp"

void Bomb::apply_explosion(Grid& grid, bool set) {
    // Onda d'urto a rombo: visita in ampiezza (BFS) dal centro, fino a
    // distanza Manhattan == range. I muri solidi bloccano la propagazione,
    // quelli distruttibili la lasciano passare (come nella vecchia croce).
    // La stessa visita e' deterministica, quindi set e unset coprono
    // esattamente le stesse celle.
    bool visited[GRID_ROWS][GRID_COLS] = {false};

    // Coda a dimensione fissa: nel caso peggiore contiene tutte le celle
    Position queue[GRID_ROWS * GRID_COLS];
    int dist[GRID_ROWS * GRID_COLS];
    int head = 0;
    int tail = 0;

    queue[tail] = p;
    dist[tail] = 0;
    tail++;
    visited[p.y][p.x] = true;

    Direction dirs[4] = {UP, LEFT, DOWN, RIGHT};

    while (head < tail) {
        Position cur = queue[head];
        int d = dist[head];
        head++;

        if (set) {
            grid.set_explosion(cur);
        }
        else {
            grid.unset_explosion(cur);
        }

        if (d == range) {
            continue;  // bordo del rombo raggiunto, non si espande oltre
        }

        for (int i = 0; i < 4; i++) {
            Position next = get_next_position(cur, dirs[i]);
            if (!visited[next.y][next.x] && grid.get_cell(next) != UNBREAKABLE_WALL) {
                visited[next.y][next.x] = true;
                queue[tail] = next;
                dist[tail] = d + 1;
                tail++;
            }
        }
    }
}

Bomb::Bomb(Position _p, int _range) {
    p = _p;
    range = _range;

    reset();

    // per convenzione
    placement_time = -1.0;
    explosion_time = -1.0;
    blink_state_start = -1.0;
}


Position Bomb::get_position() {
    return p;
}

bool Bomb::is_active() {
    return active;
}

bool Bomb::is_exploding() {
    return exploding;
}

bool Bomb::is_blinking() {
    return blink_state;
}


void Bomb::place(Position _p, int _range, double game_timer) {
    p = _p;
    range = _range;

    active = true;
    blink_state = true;

    placement_time = game_timer;
    blink_state_start = game_timer;
}


void Bomb::explode(Grid& grid, double game_timer) {
    exploding = true;
    explosion_time = game_timer;

    // Esplosione a rombo (centro incluso)
    apply_explosion(grid, true);
}


void Bomb::update(Grid& grid, double game_timer) {
    if (!exploding && placement_time - game_timer >= EXPLOSION_TIMER) {
        explode(grid, game_timer);
    }
    else if (!exploding && blink_state_start - game_timer >= BLINK_DELTA) {
        blink_state_start = game_timer;
        blink_state = !blink_state;
    }
    else if (exploding && explosion_time - game_timer >= EXPLOSION_DURATION) {
        apply_explosion(grid, false);

        reset();
    }
}

void Bomb::reset() {
    active = false;
    exploding = false;
    blink_state = false;
}