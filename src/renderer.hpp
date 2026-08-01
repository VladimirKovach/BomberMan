#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "level_manager.hpp"
#include "player.hpp"
#include "utils.hpp"

enum {
    COLOR_DEFAULT = -1,
    COLOR_GREY = 244,
};

enum {
    CP_NONE,
    CP_SCREEN,
    CP_BREAKABLE_WALL,
    CP_UNBREAKABLE_WALL,
    CP_DOOR,
    CP_PLAYER,
    CP_ENEMY,
    CP_BOMB,
    CP_EXPLOSION,
    CP_BLINK,
    CP_ITEM,
    CP_LIFE,  // rombi rossi delle vite nell'HUD
    CP_TITLE  // titolo bianco in alto
};

class Renderer {
protected:
    Position map_start_p;
    int max_y, max_x;

    void init_colors();

    void display_title();
    void display_lives(int lives);
    void display_effect(int buff_remaining);
    void display_score(int score);
    void display_time(int time);
    void display_colors_debug();

    void draw_map(Map& map);
    void draw_bombs(Bomb* bombs);
    void draw_items(Item* items);
    void draw_player(Position player_p);
    void draw_dummy_enemies(DummyEnemy* dummy_enemies);
    void draw_smart_enemies(SmartEnemy* smart_enemies);
    void draw_explosions(Map& map);

public:
    Renderer();

    void render(LevelManager& level_manager, Player& player, int score, int time);
};

#endif