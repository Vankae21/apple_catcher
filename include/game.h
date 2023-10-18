#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Basket
{
    Vector2 pos;
    Vector2 size;
    float speed;
    int apple_count;
    Texture2D tex;
} Basket;

typedef struct Apple
{
    Vector2 pos;
    Vector2 size;
    float fall_speed;
    Texture2D tex;
} Apple;

typedef struct Button
{
    Vector2 pos;
    Vector2 size;
    char* text;
    Vector2 text_pos;
    int text_size;
    Color color;
    Color hold_color;
    Color text_color;
    bool touched_over;
} Button;

typedef struct FloatingText
{
    bool active;
    Vector2 pos;
    char text[4];
    Color color;
} FloatingText;

typedef struct Tooltip
{
    Vector2 pos;
    Vector2 size;
    Color color;
    Color hold_color;
    Vector2 tooltip_pos;
    Vector2 tooltip_size;
    Color tooltip_color;
    char *text;
    int text_size;
} Tooltip;

typedef enum
{
    MAIN,
    RUN,
    PAUSE,
    LOSE
} GAME_STATE;

void draw_basket(Basket *b, float size_multiplier);
void draw_apples(Apple *a, float size_multiplier, int apple_count);
void move_apples(Apple *a, int apple_count);
Apple* init_apples(int apple_count , float max_fall_speed, float min_fall_speed, int size_multiplier, int screen_width, Texture2D apple_tex);
void draw_background(Texture2D *tex, float size_multiplier);
bool is_colliding(Basket *b, Apple *a, int offset_x, int offset_y);
void restart(Apple *apples, int apple_count, int max_fall_speed, int min_fall_speed, int screen_width);
void lose(GAME_STATE *game_state);
void draw_lose_screen(Font font, char* text, Vector2 text_pos, int font_size, Color color);
void init_button(Button *btn, Vector2 pos, Vector2 size, char* text, Vector2 text_pos, int text_size, Color color, Color hold_color, Color text_color);
bool check_button_collision(Vector2 pos, Button *btn);
void draw_button(Button *btn, Font *font);
bool is_button_pressed(Button *btn);
void init_floating_text(FloatingText *text, Vector2 pos, int point, Color color);
void update_floating_text(FloatingText *text, float text_speed, float fading_speed);
void init_tooltip(Tooltip *tooltip, Vector2 pos, Vector2 size, Color color, Color hold_color, Vector2 tooltip_pos, Vector2 tooltip_size, Color tooltip_color, char *text, int text_size);
void update_tooltip(Tooltip *tooltip, Font *font);
void start(GAME_STATE *game_state);
void go_main(GAME_STATE *game_state);

#endif