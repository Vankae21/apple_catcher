#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/game.h"
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define WIDTH 500
#define HEIGHT 800
#define SIZE_MULTIPLIER 5
#define FPS 60
#define MICRO_OFFSET 10
#define BASKET_SPEED 1200
#define MAX_APPLE_COUNT 4
#define MAX_FALL_SPEED 140
#define MIN_FALL_SPEED 300


int main()
{   
    // throw a random seed
    srand(time(0));


    InitWindow(WIDTH, HEIGHT, "GAME");    
    SetTargetFPS(FPS);

    // -------- SETUP

    Image icon = LoadImage("images/apple.png");
    SetWindowIcon(icon);

    SetExitKey(KEY_NULL);
    
    
    // -------- SETUP END
    
    // -------- GAME

    int score = 0;
    char score_text[32];
    int apple_count = 3;
    GAME_STATE game_state = MAIN;

    // malloc tooltip for info
    Tooltip *tooltip = (Tooltip*)malloc(sizeof(Tooltip));
    init_tooltip(tooltip, (Vector2){ 400, 700 }, (Vector2){ 50, 50 }, BLACK, RED, (Vector2){ -112, -72 }, (Vector2){ 224, 72 }, RAYWHITE, "CODING: Hüseyn Aga\nGRAPHICS: Elcan Zahid\nMUSIC/SFX: Hüseyn Aga", 14);

    //malloc floating text
    FloatingText *floating_text = (FloatingText*)malloc(sizeof(FloatingText));

    // create button(s)
    Button *start_button = (Button*)malloc(sizeof(Button));
    init_button(start_button, (Vector2){ WIDTH/2, HEIGHT/2 }, (Vector2){ 192, 64 }, "PLAY", (Vector2){ 50, 0 }, 32, RAYWHITE, RED, BLACK);


    Font font = LoadFontEx("fonts/Broken Console Bold.ttf", 64, NULL, 256);

    Texture2D background_texture = LoadTexture("images/background.png"); // load background texture

    // -------- GAME END

    // -------- BASKET

    //create a basket
    Basket *b = (Basket*)malloc(sizeof(Basket));

    // init the basket

    Texture2D basket_textures[4] = { LoadTexture("images/basket_0.png"), LoadTexture("images/basket_1.png"), LoadTexture("images/basket_2.png"), LoadTexture("images/basket_3.png")};

    b->tex = basket_textures[0];
    b->size = (Vector2){ 32 * SIZE_MULTIPLIER, 21 * SIZE_MULTIPLIER };
    b->apple_count = 0;
    b->pos = (Vector2){(WIDTH - b->size.x)/2, 635};
    b->speed = BASKET_SPEED;

    // ---------- BASKET END

    // ---------- APPLES



    // init the apples
    Texture2D apple_tex = LoadTexture("images/apple.png");

    Apple *apples = init_apples(apple_count, MAX_FALL_SPEED, MIN_FALL_SPEED, SIZE_MULTIPLIER, WIDTH, apple_tex);


    // ---------- APPLES END

    while(!WindowShouldClose())
    {

        if(game_state == PAUSE)
        {
            if(IsKeyReleased(KEY_SPACE)) 
                game_state = RUN;
            else if(IsKeyReleased(KEY_ESCAPE))
                go_main(&game_state);
        }

        else if(game_state == LOSE)
        {
            update_floating_text(floating_text, 100.0f, 100.0f); //move floating text
                        
            if(IsKeyReleased(KEY_R))
            {
                floating_text->active = false;
                floating_text->color = BLANK;
                score = 0;
                b->apple_count = 0;
                b->tex = basket_textures[0];
                game_state = RUN;
                restart(apples, apple_count, MAX_FALL_SPEED, MIN_FALL_SPEED, WIDTH);
            }
        }
        else if(game_state == RUN)
        {
            if(IsKeyReleased(KEY_ESCAPE))
                game_state = PAUSE;

            if(b->pos.x + MICRO_OFFSET < (GetMousePosition().x - b->size.x/2))
                b->pos.x += b->speed * GetFrameTime();
            else if(b->pos.x - MICRO_OFFSET > (GetMousePosition().x - b->size.x/2))
                b->pos.x -= b->speed * GetFrameTime();

            move_apples(apples, apple_count);    // apple movement here:
            
            update_floating_text(floating_text, 100.0f, 100.0f); //move floating text

            for(int i = 0; i < apple_count; i++)
            {
                if(is_colliding(b, &(apples[i]), 0, -10)) // apple got catched
                {
                    score++;

                    if(b->apple_count + 1 < MAX_APPLE_COUNT)
                        b->apple_count++;
                    else
                    {
                        b->apple_count = 0;
                        int rand_point = (rand() % 10) + 1;
                        init_floating_text(floating_text, b->pos, rand_point, BLACK);
                        score += rand_point;
                    }

                    b->tex = basket_textures[b->apple_count];

                    apples[i].pos.y = -apples[i].size.y;
                    int rand_x = (rand() % (int)(WIDTH - apples[i].size.x));
                    apples[i].pos.x = rand_x;
                    apples[i].fall_speed = (rand() % (MAX_FALL_SPEED - MIN_FALL_SPEED + 1)) + MIN_FALL_SPEED;
                }

                if(apples[i].pos.y > HEIGHT)
                {
                    apples[i].pos.y = -apples[i].size.y;
                    int rand_x = (rand() % (int)(WIDTH - apples[i].size.x));
                    apples[i].pos.x = rand_x;
                    apples[i].fall_speed = (rand() % (MAX_FALL_SPEED - MIN_FALL_SPEED + 1)) + MIN_FALL_SPEED;

                    // lose game here
                    lose(&game_state);
                }
            }
        }
        else if(game_state == MAIN)
        {
            if(is_button_pressed(start_button))
            {
                start(&game_state);
                restart(apples, apple_count, MAX_FALL_SPEED, MIN_FALL_SPEED, WIDTH);
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if(game_state == PAUSE)
        {
            draw_background(&background_texture, SIZE_MULTIPLIER);
            draw_basket(b, SIZE_MULTIPLIER);
            draw_apples(apples, SIZE_MULTIPLIER, apple_count);

            DrawTextEx(font, score_text, (Vector2){ 10, 30 }, 32, 0, RAYWHITE);
            DrawTextEx(font, "                   PAUSED\n     PRESS SPACE TO RESUME\nPRESS ESC TO GO MAIN MENU", (Vector2){ 0, 320 }, 32, 0, BLACK);
        }
        else if(game_state == RUN)
        {
            sprintf(score_text, "Score: %d", score);

            draw_background(&background_texture, SIZE_MULTIPLIER);
            draw_basket(b, SIZE_MULTIPLIER);
            draw_apples(apples, SIZE_MULTIPLIER, apple_count);

            DrawTextEx(font, score_text, (Vector2){ 10, 30 }, 32, 0, RAYWHITE);
            DrawTextEx(font, floating_text->text, floating_text->pos, 64, 0, floating_text->color); // draw the floating text


            // check collision
            

            // change position after it has been drawn
        }
        else if(game_state == LOSE)
        {
            draw_background(&background_texture, SIZE_MULTIPLIER);
            draw_basket(b, SIZE_MULTIPLIER);

            DrawTextEx(font, floating_text->text, floating_text->pos, 64, 0, floating_text->color); // draw the floating text
            
            draw_lose_screen(font, "          GAME OVER\n  press r for restart", (Vector2){ 50 , 250 }, 32, RAYWHITE);
            DrawTextEx(font, score_text, (Vector2){ 10, 30 }, 32, 0, RAYWHITE);
        }
        else if(game_state == MAIN)
        {
            draw_background(&background_texture, SIZE_MULTIPLIER);
            draw_basket(b, SIZE_MULTIPLIER);
            draw_button(start_button, &font);
            update_tooltip(tooltip, &font);
        }
        EndDrawing();
    
        srand(time(0) % rand());     // randomize the seed
    }

    CloseWindow();

    UnloadTexture(b->tex); // unload textures
    UnloadTexture(apple_tex);
    UnloadTexture(background_texture);
    UnloadImage(icon);
    
    UnloadFont(font); //unload the font

    free(b); // free the basket
    free(apples); // free the apple(s)
    free(start_button);
    free(floating_text);

    return 0;
}