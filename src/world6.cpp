#include "world6.h"
#include "bosses.h"
#include <iostream>


namespace World6 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    int boss_scale;

    int battery_life;

    int door_height;
    int door_width;

    int window_height;
    int window_width;
    float size;
    int texturesize;

    Texture2D texture;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        

        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;
        battery_life = 1000;
        
        texturesize = 100;
        size = .1;

 
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++; // just updating score every frame for some reason

        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);


        if (IsKeyDown(KEY_SPACE)) {
            currentBoss.health -= 10;

        }

        //boss health
        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }
       
        //world complete
        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;

       
    }
    void Draw(const GameState& game) {
        //drawing image
        Image image = LoadImage("assets/images/blackcat.jpg");     // Loads to RAM
        texture = LoadTextureFromImage(image);    // Transfers to GPU VRAM
        UnloadImage(image);
        Vector2 CatPosition = { 100 , 200 };

        DrawTextureEx(texture, CatPosition, 0, size, RAYWHITE);
      
        //positions
        int text_x = 100;
        int text_y = 100;

        int door_height = 300;
        int door_width = 200;
        int door_x = 100;
        int door_y = 200;

        int window_height = 200;
        int window_width = 200;
        int window_x = 500;
        int window_y = 200;

        int door_cat_x = 150;
        int door_cat_y = 250;

        int window_cat_x;
        int windo_cat_y;



        //images
        //DrawTexture (texture, door_cat_x, door_cat_y, WHITE);

        //Level Text
        DrawText("Battery Life -", text_x, text_y, 20, WHITE);
        DrawText("E to Close Window", window_x, 550, 20, WHITE);
        DrawText("Q to Close Door", door_x, 550, 20, WHITE);
        
        //Shapes
        DrawRectangleLines(100,200, door_width, door_height, PINK);
        DrawRectangleLines(500, 200, window_width, window_height, BLUE);
        
        //battery life bar
        DrawRectangleLines(250, 100, 500, 20, RED);
        DrawRectangle(250, 100, battery_life / 2, 20, RED);
        
        //door
        if (IsKeyDown(KEY_Q)&& battery_life > 0) {
            battery_life-= 1;
            DrawRectangle(door_x, door_y, door_width, door_height, PINK);
        }
        else if (IsKeyReleased(KEY_Q)) {
            DrawRectangleLines(door_x, door_y, door_width, door_height, PINK);
        }
        //Window
        if (IsKeyDown(KEY_E)&& battery_life > 0) {
            battery_life -= 1;
            DrawRectangle(window_x, window_y, window_width, window_height, BLUE);
        }
        else if (IsKeyReleased(KEY_E)) {
            DrawRectangleLines(window_x, window_y, window_width, window_height, BLUE);
        }
        

        //get the current boss
        /*const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale;

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
        */
        
    }
}