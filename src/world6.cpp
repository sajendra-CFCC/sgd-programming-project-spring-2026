#include "world6.h"
#include "bosses.h"
#include <iostream>

namespace World6 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    int boss_scale;

    int battery_life;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        

        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;
        battery_life = 1000;
        
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++; // just updating score every frame for some reason

        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);

        if (IsKeyDown(KEY_SPACE)) {
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
        int text_x = 100;
        int text_y = 100;
        //Level Text
        DrawText("Walmart FNAF", text_x, text_y, 20, WHITE);
        DrawText("E to Close Window", 550, 500, 20, WHITE);
        DrawText("Q to Close Door", 50, 500, 20, WHITE);
        //Shapes
        DrawRectangleLines(100,200, 100, 200, PINK);
        DrawRectangleLines(550, 200, 100, 100, BLUE);
        
        DrawCircle(150,450,15,PINK);
        DrawCircle(600, 450, 15, BLUE);
        
        DrawRectangleLines(250, 100, 500, 20, RED);
        DrawRectangle(250, 100, battery_life / 2, 20, RED);
        //door
        if (IsKeyDown(KEY_Q)&& battery_life > 0) {
            battery_life-= 1;
            DrawRectangle(100, 200, 100, 200, PINK);
        }
        else if (IsKeyReleased(KEY_Q)) {
            DrawRectangleLines(100, 200, 100, 200, PINK);
        }
        //Window
        if (IsKeyDown(KEY_E)&& battery_life > 0) {
            battery_life -= 1;
            DrawRectangle(550, 200, 100, 100, BLUE);
        }
        else if (IsKeyReleased(KEY_E)) {
            DrawRectangleLines(550, 200, 100, 100, BLUE);
        }

        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale;

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);

        
    }
}