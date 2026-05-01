#include "world6.h"
#include "bosses.h"
#include <iostream>


namespace World6 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    float boss_scale;
    float boss_grow_rate = 0.01;

    int boss2_x;
    int boss2_y;
    float boss2_scale;
    float boss2_grow_rate = 0.01;

    int battery_life;

    int door_x = 100;
    int door_y = 200;
    int door_height = 300;
    int door_width = 200;

    int window_x = 500;
    int window_y = 200;
    int window_height = 200;
    int window_width = 200;
    
    float size;
    int texturesize;

    Texture2D texture;
    Texture2D windowTexture;

    //represents if door and window currently open or closed
    bool door_closed = false;
    bool window_closed = false; 


    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        

        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;
        boss2_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss2_scale = 1;
        battery_life = 800;
    }

    WorldUpdateResult Update(GameState& game) {
        //get the current boss
        BossState& currentBoss = Bosses::ActiveBoss(game);
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        Rectangle bossHB2 = Bosses::GetHitbox(currentBoss, boss2_x, boss2_y, boss2_scale);
        //check for input
        if (IsKeyDown(KEY_SPACE)) {
            currentBoss.health -= 10;
        }

        if (IsKeyDown(KEY_Q) && battery_life > 0) {
            battery_life -= 1;
            door_closed = true;
            boss_scale = 1;
            size = 0;
        } else {
            door_closed = false;
        }

        if (IsKeyDown(KEY_E) && battery_life > 0) {
            battery_life -= 1;
            window_closed = true;
            boss2_scale = 1;
        } else {
            window_closed = false;
        }


        //update state
        game.score++; // just updating score every frame for some reason
       
        //hitboxes
        Rectangle doorHB = { door_x, door_y};
        Rectangle windowHB = { window_x, window_y };
       
        //collisions
        bool DoorCollision = CheckCollisionRecs(bossHB, doorHB);
        if (DoorCollision == true) {
            printf("collision\n"); 
            game.health -= 15;
            boss_scale = 1;
            size = 0;
        }
        else if (DoorCollision == false) {
            boss_scale *= (1 + boss_grow_rate);
        }
        bool WindowCollision = CheckCollisionRecs(bossHB2, windowHB);
        if (WindowCollision == true) {
            printf("collision\n");
            game.health -= 15;
            boss2_scale = 1;
        }
        else if (WindowCollision == false) {
            boss2_scale *= (1 + boss_grow_rate);
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

        int door_cat_x = 125;
        int door_cat_y = 290;
        int window_cat_x = 550;
        int window_cat_y = 200;
        //drawing door cat
        Image image = LoadImage("assets/images/blackcat.jpg");     // Loads to RAM
        texture = LoadTextureFromImage(image);    // Transfers to GPU VRAM
        UnloadImage(image);
        Vector2 CatPosition = { door_cat_x , door_cat_y };

        DrawTextureEx(texture, CatPosition, 0, size, RAYWHITE);
        //drawing window cat
        Image Windowimage = LoadImage("assets/images/WindowCat.jpg");     // Loads to RAM
        windowTexture = LoadTextureFromImage(Windowimage);    // Transfers to GPU VRAM
        UnloadImage(Windowimage);
        Vector2 CatWindowPosition = { window_cat_x , window_cat_y };

        DrawTextureEx(windowTexture, CatWindowPosition, 0, size, RAYWHITE);

        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        int boss_radius = Bosses::GetHitRadius(currentBoss, boss_scale);
        Rectangle bossHB2 = Bosses::GetHitbox(currentBoss, boss2_x, boss2_y, boss2_scale);
        size += .002;

        boss_x = 200;
        boss_y = 350;
        boss2_x = 600;
        boss2_y = 300;


        //Level Text
        //positions
        int text_x = 100;
        int text_y = 100;
        DrawText("Battery Life -", text_x, text_y, 20, WHITE);
        DrawText("E to Close Window", window_x, 550, 20, WHITE);
        DrawText("Q to Close Door", door_x, 550, 20, WHITE);

        //Shapes
        DrawRectangleLines(100, 200, door_width, door_height, PINK);
        DrawRectangleLines(500, 200, window_width, window_height, BLUE);
                
        //battery life bar
        DrawRectangleLines(250, 100, 400, 20, RED);
        DrawRectangle(250, 100, battery_life / 2, 20, RED);
        
        //door
        if (door_closed) {
            DrawRectangle(door_x, door_y, door_width, door_height, PINK);
        } else {
            DrawRectangleLines(door_x, door_y, door_width, door_height, PINK);
        }
        //Window
        if (window_closed) {
            DrawRectangle(window_x, window_y, window_width, window_height, BLUE);
        }
        else {
            DrawRectangleLines(window_x, window_y, window_width, window_height, BLUE);
        }

       //Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
       //visualize hitbox for testing
       //DrawRectangleLinesEx(bossHB, 1, RED);
       //DrawRectangleLinesEx(bossHB2, 1, BLUE);
       // DrawCircleLines(boss_x, boss_y, boss_radius, GREEN);

    }
}