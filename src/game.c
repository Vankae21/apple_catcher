#include "raylib.h"
#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void draw_basket(Basket *b, float size_multiplier)
{
    DrawTextureEx(b->tex, b->pos, 0, size_multiplier, RAYWHITE);
}
Apple* init_apples(int apple_count , float max_fall_speed, float min_fall_speed, int size_multiplier, int screen_width, Texture2D apple_tex)
{
    Apple *apples = (Apple*)malloc(sizeof(Apple) * apple_count);
    
    for(int i = 0; i < apple_count; i++)
    {
        apples[i].tex = apple_tex;
        apples[i].fall_speed = (i + 1) * 100; // apples fall speed
        printf("Speed: %f", apples[i].fall_speed);
        apples[i].size = (Vector2){ 10 * size_multiplier, 11 * size_multiplier };
        apples[i].pos = (Vector2){ (rand() % (int)(screen_width - apples[i].size.x)) , -apples[i].size.y };
    }

    return apples;
}

void draw_apples(Apple *a, float size_multiplier, int apple_count)
{
    for(int i = 0; i < apple_count; i++)
    {
        DrawTextureEx(a[i].tex, a[i].pos, 0, size_multiplier,RAYWHITE);
    }
}

void move_apples(Apple *a, int apple_count)
{
    for(int i = 0; i < apple_count; i++)
    {
        a[i].pos.y += a[i].fall_speed * GetFrameTime();
    }
}

void draw_background(Texture2D *tex, float size_multiplier)
{
    DrawTextureEx(*tex, (Vector2){0, 0}, 0, size_multiplier, RAYWHITE);
}

bool is_colliding(Basket *b, Apple *a, int offset_x, int offset_y)
{
    if(a->pos.y - a->size.y - offset_y > b->pos.y &&
    (a->pos.x > b->pos.x && a->pos.x + a->size.x < b->pos.x + b->size.x) &&
    (a->pos.y < b->pos.y + b->size.y)
    ) return true;


    return false;
}

void restart(Apple *apples, int apple_count, int max_fall_speed, int min_fall_speed, int screen_width)
{
    for(int i = 0; i < apple_count; i++)
    {
        apples[i].fall_speed = (rand() % (int)(max_fall_speed - min_fall_speed + 1)) + min_fall_speed;
        apples[i].pos = (Vector2){ (rand() % (int)(screen_width - apples[i].size.x)) , -apples[i].size.y };
    }
}

void lose(GAME_STATE *game_state)
{
    *game_state = LOSE;
}

void draw_lose_screen(Font font,char* text, Vector2 text_pos, int font_size, Color color)
{
    DrawTextEx(font, text, text_pos,font_size, 0, color);
}

bool check_button_collision(Vector2 pos, Button *btn)
{
    if(pos.x <= btn->pos.x + btn->size.x &&
    pos.x >= btn->pos.x &&
    pos.y <= btn->pos.y + btn->size.y &&
    pos.y >= btn->pos.y)
        return true;
    return false;
}

void init_button(Button *btn, Vector2 pos, Vector2 size, char* text, Vector2 text_pos, int text_size, Color color, Color hold_color, Color text_color)
{
    btn->size = size;
    btn->pos.x = pos.x - size.x/2;
    btn->pos.y = pos.y - size.y/2;
    btn->text = text;
    btn->text_size = text_size;
    btn->text_pos.x = btn->pos.x + text_pos.x;
    btn->text_pos.y = btn->pos.y + btn->size.y/2 - text_size/2 + text_pos.y;
    btn->color = color;
    btn->hold_color = hold_color;
    btn->text_color = text_color;
    btn->touched_over = false;
}

void draw_button(Button *btn, Font *font)
{
    Color color;
    if(check_button_collision(GetMousePosition(), btn))
    {
        color = btn->hold_color;
    }
    else
    {
        color = btn->color;
    }
    DrawRectangle(btn->pos.x, btn->pos.y, btn->size.x, btn->size.y, color);
    DrawTextEx(*font, btn->text, btn->text_pos, btn->text_size, 0, btn->text_color);
}

bool is_button_pressed(Button *btn)
{
    if(!check_button_collision(GetMousePosition(), btn))
    {
        btn->touched_over = false;
        return false;
    }
    if(IsMouseButtonPressed(0) && !btn->touched_over)
    {
        btn->touched_over = true;
    }
    else if(IsMouseButtonReleased(0) && btn->touched_over)
    {
        btn->touched_over = false;
        return true;
    }
    return false;
}

void init_floating_text(FloatingText *text, Vector2 pos, int point, Color color)
{
    char point_text[4];
    text->pos = pos;
    text->color = color;
    sprintf(point_text, "+%d", point);
    for(int i = 0; i < sizeof(text->text)/sizeof(char); i++)
    {
        text->text[i] = point_text[i];
    }
    text->active = true;
}

void update_floating_text(FloatingText *text, float text_speed, float fading_speed)
{
    if(!text->active) return;
    if(text->color.a <= 0) return;
    text->pos.y -= text_speed * GetFrameTime();
    text->color.a -= fading_speed * GetFrameTime();
}

void init_tooltip(Tooltip *tooltip, Vector2 pos, Vector2 size, Color color, Color hold_color, Vector2 tooltip_pos, Vector2 tooltip_size, Color tooltip_color, char *text, int text_size)
{
    tooltip->pos = pos;
    tooltip->size = size;
    tooltip->color = color;
    tooltip->hold_color = hold_color;
    tooltip->tooltip_pos.x = tooltip->pos.x + tooltip_pos.x;
    tooltip->tooltip_pos.y = tooltip->pos.y + tooltip_pos.y;
    tooltip->tooltip_size = tooltip_size;
    tooltip->tooltip_color = tooltip_color;
    tooltip->text = text;
    tooltip->text_size = text_size;
}

void update_tooltip(Tooltip *tooltip, Font *font)
{
    
    if(GetMousePosition().x <= tooltip->pos.x + tooltip->size.x &&
    GetMousePosition().x >= tooltip->pos.x &&
    GetMousePosition().y <= tooltip->pos.y + tooltip->size.y &&
    GetMousePosition().y >= tooltip->pos.y)
    {
        DrawRectangle(tooltip->pos.x, tooltip->pos.y, tooltip->size.x, tooltip->size.y, tooltip->hold_color); // draw button
        DrawRectangle(tooltip->tooltip_pos.x, tooltip->tooltip_pos.y, tooltip->tooltip_size.x, tooltip->tooltip_size.y, tooltip->tooltip_color); // draw tooltip box
        DrawTextEx(*font, tooltip->text, (Vector2){tooltip->tooltip_pos.x + 4, tooltip->tooltip_pos.y + 8}, tooltip->text_size, 0, BLACK); // draw text
    }
    else
    {
        DrawRectangle(tooltip->pos.x, tooltip->pos.y, tooltip->size.x, tooltip->size.y, tooltip->color);
    }
}

void start(GAME_STATE *game_state)
{
    *game_state = RUN;
}
void go_main(GAME_STATE *game_state)
{
    *game_state = MAIN;
}