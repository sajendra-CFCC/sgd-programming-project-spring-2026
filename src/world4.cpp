#include "world4.h"
#include "bosses.h"
#include <iostream>

namespace World4 {
    bool world_complete;
        

    int PlayerX = 400;
    int PlayerY = 300;

    int boss_x;
    int boss_y;
    int boss_scale;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        
        int PlayerX = 400;
        int PlayerY = 300;
       

        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;
        
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++; // just updating score every frame for some reason
       

        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);
        
        if (IsKeyPressed(KEY_SPACE)) {
            currentBoss.health -= 10;
            
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
   
        DrawTriangle(Vector2 {400,100},Vector2 {300,300},Vector2 {500,300},RED);
       
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