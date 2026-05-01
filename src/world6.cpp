#include "world6.h"
#include "bosses.h"
#include <iostream>


namespace World6 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    float boss_scale;
    float boss_grow_rate = 0.003;

    int battery_life;

    int door_x = 100;
    int door_y = 200;
    int door_height = 300;
    int door_width = 200;

    int window_x = 500;
    int window_y = 200;
    int window_height = 200;
    int window_width = 200;
    
    /*float size;
    int texturesize;

    Texture2D texture;*/

    //represents if door and window currently open or closed
    bool door_closed = false;
    bool window_closed = false; 


    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        

        boss_x = SCREEN_WIDTH / 4;
        boss_y = SCREEN_HEIGHT / 4;
        boss_scale = 1;
        battery_life = 1000;
        
        /*texturesize = 100;
        size = .1;*/
 
    }

    WorldUpdateResult Update(GameState& game) {
        //get the current boss
        BossState& currentBoss = Bosses::ActiveBoss(game);
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        
        //check for input
        if (IsKeyDown(KEY_SPACE)) {
            currentBoss.health -= 10;
        }

        if (IsKeyDown(KEY_Q) && battery_life > 0) {
            battery_life -= 1;
            door_closed = true;
            boss_scale = 1;
        } else {
            door_closed = false;
        }

        if (IsKeyDown(KEY_E) && battery_life > 0) {
            battery_life -= 1;
            window_closed = true;
        } else {
            window_closed = false;
        }


        //update state
        game.score++; // just updating score every frame for some reason
        //increase boss scale
        //boss_scale *= (1 + boss_grow_rate);

        //Collision stuff
        //NOTE: there is a mistake here, but you need to check collision logic if you change this
        //NOTE lets discuss in class
        Rectangle doorHB = { door_x, door_y };
        bool DoorCollision = CheckCollisionRecs(bossHB, doorHB);
        if (DoorCollision == true) {
            printf("collision\n"); 
            game.health = 0;
        }
        else if (DoorCollision == false) {
            boss_scale *= (1 + boss_grow_rate);
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
        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        int boss_radius = Bosses::GetHitRadius(currentBoss, boss_scale);

        //drawing image
        //Image image = LoadImage("assets/images/blackcat.jpg");     // Loads to RAM
        //texture = LoadTextureFromImage(image);    // Transfers to GPU VRAM
        //UnloadImage(image);
        //Vector2 CatPosition = { 100 , 250 };

        //DrawTextureEx(texture, CatPosition, 0, size * 2, RAYWHITE);

        //positions
        int text_x = 100;
        int text_y = 100;

        
        /*int door_cat_x = 150;
        int door_cat_y = 250;*/

        //bool DoorCollision = false;

        /*int window_cat_x;
        int windo_cat_y;
        size += .0001;*/

        boss_x = 200;
        boss_y = 350;


        //Level Text
        DrawText("Battery Life -", text_x, text_y, 20, WHITE);
        DrawText("E to Close Window", window_x, 550, 20, WHITE);
        DrawText("Q to Close Door", door_x, 550, 20, WHITE);

        //Shapes
        DrawRectangleLines(100, 200, door_width, door_height, PINK);
        DrawRectangleLines(500, 200, window_width, window_height, BLUE);
                
        //battery life bar
        DrawRectangleLines(250, 100, 500, 20, RED);
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
        } else {
            DrawRectangleLines(window_x, window_y, window_width, window_height, BLUE);
        }
        
        //float boss_size = size * 20;
        //boss_scale = boss_size;

       Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
       //Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size *2);
       //visualize hitbox for testing
       DrawRectangleLinesEx(bossHB, 1, RED);
       // DrawCircleLines(boss_x, boss_y, boss_radius, GREEN);

    }
}