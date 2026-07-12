#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Map.hpp"
#include "Player.hpp"

enum Color {
    ORANGE = 208,
    GREY = 244
};

enum ColorPair {
    CP_DEFAULT,  // colore predefinito del terminale
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
    CP_LIFE,   // rombi rossi delle vite nell'HUD
    CP_TITLE,  // titolo bianco in alto
};

class Renderer {
protected:
    Position map_start_p;
    int max_y, max_x;

    void init_colors();
    void paint_it_black();

    void display_title();
    void display_lives(int lives);
    void display_effect(double buff_remaining);
    void display_score(int score);
    void display_time(double time);
    void display_colors_debug();

    char get_cell_view(Cell c);
    ColorPair get_cell_color(Cell c);

    void draw_grid(Grid& grid);
    void draw_bombs(Bomb* bombs);
    void draw_items(Item* items);
    void draw_player(Position player_p);
    void draw_dummy_enemies(DummyEnemy* dummy_enemies);
    void draw_smart_enemies(SmartEnemy* smart_enemies);
    void draw_explosions(Grid& grid);

public:
    Renderer();

    void render(Map& map, Player& player, int score, int time, double game_clock);
};

#endif