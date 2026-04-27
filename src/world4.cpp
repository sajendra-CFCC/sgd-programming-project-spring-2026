#include "world4.h"
#include "bosses.h"
#include <iostream>


namespace World4 {
    bool world_complete;
        

    Vector2 playerPosition = { 400, 300 };
    Vector2 playerSpeed = { 100.0f, 100.0f };
    int playerSize = 20;
    int playerRotation = 0;
    int boss_x;
    int boss_y;
    int boss_scale;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        playerPosition.x = SCREEN_WIDTH / 2;
        playerPosition.y = SCREEN_HEIGHT - playerSize;
               

        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;
        
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++; // just updating score every frame for some reason
       

        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);


        //Bullet stuff
        struct bullet {
            Vector2 Bullet_Possition ;
            Vector2 Bullet_Velocity;
            float radius;
            
        };
        
        if (IsKeyPressed(KEY_SPACE)) {
            bullet b;
            b.Bullet_Possition = playerPosition;
            float speed = 500.0f;
            b.Bullet_Velocity = { speed , 0 };
            b.radius = 5.0;
            DrawCircle(playerPosition.x, playerPosition.y, 50, RED); 
            currentBoss.health -= 10;

        }

        


       //Player Movement
        
        if (IsKeyDown(KEY_RIGHT)) playerPosition.x += 6.0f;
        if (IsKeyDown(KEY_LEFT)) playerPosition.x -= 6.0f;
        if (IsKeyDown(KEY_UP)) playerPosition.y -= 8.0f;
        if (IsKeyDown(KEY_DOWN)) playerPosition.y += 6.0f;
        if (currentBoss.health <= 0) {
        if (IsKeyDown(KEY_LEFT)) playerRotation -= 5;
        if (IsKeyDown(KEY_RIGHT)) playerRotation += 5;
        
          
      

      
        



            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }
       

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        //do the drawing for your world here
        
        
        //DrawTriangle(Vector2 {400,100},Vector2 {300,300},Vector2 {500,300},RED);
        DrawPoly(playerPosition, 3, playerSize, -90, GREEN);
        
        int text_x = 100;
        int text_y = 100;
        DrawText("Template World - Press SPACE to hit boss", text_x, text_y, 20, WHITE);

        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale;

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);


        
    }
}