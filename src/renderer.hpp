#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "bomb.hpp"
#include "item.hpp"
#include "level_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "position.hpp"
#include <ncurses.h>

// Larghezza del pannello laterale con LEVEL / LIVES / EFFECT / SCORE / TIME
const int INFO_WIDTH = 20;

enum {
    COLOR_DEFAULT = -1,
    COLOR_GREY = 244,
};

enum {
    CP_NONE,
    CP_SCREEN,
    CP_WALL_SOLID,
    CP_WALL_DESTRUCTIBLE,
    CP_DOOR,
    CP_PLAYER,
    CP_ENEMY,
    CP_BOMB,
    CP_BOMB_BLINK,
    CP_EXPLOSION,
    CP_ITEM,
    CP_TITLE,
};

class Renderer {
protected:
    int map_start_y, map_start_x;
    int max_y, max_x;

    WINDOW* map_window;
    WINDOW* info_window;

    void init_colors();

    void display_title();

    void draw_grid(Map& map);
    void draw_bombs(Bomb* bombs);
    void draw_items(Item* items);
    void draw_player(Player& player);
    void draw_enemies(Level& level);
    void draw_explosions(Map& map);

    void display_lives(int lives);
    void display_effect(int buff_remaining);

    void draw_map(Level& level, Player& player);
    void draw_info(Level& level, Player& player, int score, int time);

public:
    Renderer();
    ~Renderer();

    void render(LevelManager& level_manager, Player& player, int score, int time);
};

#endif