#include "world4.h"
#include "bosses.h"
#include <iostream>


namespace World4 {
    bool world_complete;
    //Bullet stuff
    struct bullet {
        bool active = false;
        Vector2 Bullet_Possition = { 0 };
        Vector2 Bullet_Velocity = { 0 };
        float radius;

    };

    Vector2 playerPosition = { 400, 300 };
    Vector2 playerSpeed = { 100.0f, 100.0f };
    int playerSize = 20;
    int playerRotation = 0;
    int boss_x;
    int boss_y;
    int boss_scale;
    //static int shootMissle = 0;
    //Vector2 bulletPos = { 0,0 };
    //bool bulletActive = false;

   
    


    bullet b;

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


        
        
        if (IsKeyPressed(KEY_SPACE) && b.active == false ) {
            float speed = 20.0f;
            b.Bullet_Possition = playerPosition;
            b.Bullet_Velocity = { 0 , -speed };
            b.radius = 10.0;
            b.active = true;
            
            //currentBoss.health -= 10;


        }

        
        if (b.active) {
            b.Bullet_Possition.x += b.Bullet_Velocity.x;
            b.Bullet_Possition.y += b.Bullet_Velocity.y;
            if (b.Bullet_Possition.y < 0) {
                b.active = false;
            }
        }


       //Player Movement
        
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerPosition.x += 6.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) playerPosition.x -= 6.0f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) playerPosition.y -= 8.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) playerPosition.y += 6.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) playerRotation -= 5;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerRotation += 5;
        //if (IsKeyDown(KEY_SPACE)) shootMissle = 1;
        /*bullet b;*/

        //get the boss hitbox
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, boss_x, boss_y, boss_scale);
        if( CheckCollisionCircleRec(b.Bullet_Possition, b.radius, bossHB) && b.active ) {
            std::cout << "BOOM!\n";
            b.Bullet_Velocity.x = GetRandomValue(-5, 5);
            currentBoss.health -= 10;
            b.active = false;
        }


      
        


        if (currentBoss.health <= 0) {
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

        if(b.active) {
            DrawCircleV(b.Bullet_Possition, b.radius, RED);
        }

        
    }
}