#include "world6.h"
#include "bosses.h"
#include <iostream>


namespace World6 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    float boss_scale;
    float boss_grow_rate = 0.005;

    int boss2_x;
    int boss2_y;
    float boss2_scale;
    float boss2_grow_rate = 0.005;

    int battery_life;

    int door_x = 100;
    int door_y = 200;
    int door_height = 300;
    int door_width = 200;

    int window_x = 500;
    int window_y = 200;
    int window_height = 200;
    int window_width = 200;
    
    float sizeDoor;
    float sizeWindow;
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

        //load door cat
        Image image = LoadImage("assets/images/blackcat.jpg");     // Loads to RAM
        texture = LoadTextureFromImage(image);    // Transfers to GPU VRAM
        UnloadImage(image);

        //load window cat
        Image Windowimage = LoadImage("assets/images/WindowCat.png");     // Loads to RAM
        windowTexture = LoadTextureFromImage(Windowimage);    // Transfers to GPU VRAM
        UnloadImage(Windowimage);


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

        if (IsKeyDown(KEY_Q) && battery_life > 0 && window_closed == false) {
            battery_life -= 2;
            door_closed = true;
            sizeDoor = 0;
            window_closed = false;
        } else {
            door_closed = false;
        }

        if (IsKeyDown(KEY_E) && battery_life > 0 && door_closed == false) {
            battery_life -= 2;
            window_closed = true;
            sizeWindow = 0;
            door_closed = false;
        } else {
            window_closed = false;
        }


        //update state
        game.score++; // just updating score every frame for some reason
       
        //hitboxes
        Rectangle doorHB = { (float)door_x, (float)door_y};
        Rectangle windowHB = { (float)window_x, (float)window_y };
       
        //collisions
        bool DoorCollision = CheckCollisionRecs(bossHB, doorHB);
        if (DoorCollision == true) {
            printf("collision\n"); 
            game.health -= 10;
            boss_scale = 1;
            sizeDoor = 0;
        }
        else if (DoorCollision == false) {
            boss_scale *= (1 + boss_grow_rate);
        }
        bool WindowCollision = CheckCollisionRecs(bossHB2, windowHB);
        if (WindowCollision == true) {
            printf("collision\n");
            game.health -= 10;
            boss2_scale = 1;
            sizeWindow = 0;
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
        int window_cat_x = 500;
        int window_cat_y = 200;
        //drawing door cat
        Vector2 CatPosition = { (float)door_cat_x , (float)door_cat_y };
        DrawTextureEx(texture, CatPosition, 0, sizeDoor, RAYWHITE);
        
        //drawing window cat
        Vector2 CatWindowPosition = { (float)window_cat_x , (float)window_cat_y };
        DrawTextureEx(windowTexture, CatWindowPosition, 0, sizeWindow, RAYWHITE);

        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        int boss_radius = Bosses::GetHitRadius(currentBoss, boss_scale);
        Rectangle bossHB2 = Bosses::GetHitbox(currentBoss, boss2_x, boss2_y, boss2_scale);
        sizeDoor += .002;
        sizeWindow += .002;

        boss_x = 200;
        boss_y = 350;
        boss2_x = 600;
        boss2_y = 300;


        //Level Text
        //positions
        int text_x = 100;
        int text_y = 100;
        int level_text_x = 35;
        int level_text_y = 150;
        DrawText("Block the bosses from getting to you! Shut them out before they escape!", level_text_x, level_text_y, 20, RED);
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
            boss_scale = 1;
        } else {
            DrawRectangleLines(door_x, door_y, door_width, door_height, PINK);
            boss_scale *= (1 + boss_grow_rate);
        }
        //Window
        if (window_closed) {
            DrawRectangle(window_x, window_y, window_width, window_height, BLUE);
            boss2_scale = 1;
        }
        else {
            DrawRectangleLines(window_x, window_y, window_width, window_height, BLUE);
            boss2_scale *= (1 + boss_grow_rate);
        }

       //Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
       //visualize hitbox for testing
       //DrawRectangleLinesEx(bossHB, 1, RED);
       //DrawRectangleLinesEx(bossHB2, 1, BLUE);
       // DrawCircleLines(boss_x, boss_y, boss_radius, GREEN);

    }
}