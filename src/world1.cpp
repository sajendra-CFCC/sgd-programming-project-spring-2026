#include "world1.h"
#include "bosses.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace World1 {
    bool world_complete;
    bool spawn_rand;
    bool draw_spot;
    int text_x;
    int text_y;
    int boss_x;
    int boss_y;
    int boss_scale;
    Vector2 circ_pos;
    Vector2 rand_pos;
    
    const int maxBullets = 15;
    int numBullets = 5;
    Rectangle bullets[maxBullets] = {0};
    bool bull_rand[maxBullets] = { false };

    void Init() {
        world_complete = false;
        spawn_rand = false;
        draw_spot = true;
        text_x = 145;
        text_y = 550;
        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2 - 200;
        boss_scale = 1;
        std::srand(std::time(0));
        rand_pos.x = SCREEN_WIDTH / 2;
        rand_pos.y = SCREEN_HEIGHT / 2 + 200;
        circ_pos.x = SCREEN_WIDTH / 2;
        circ_pos.y = SCREEN_HEIGHT / 2;

        for (int i = 0; i < numBullets; i++) {
            bullets[i].x = i * SCREEN_WIDTH / numBullets;
            bullets[i].y = 20;
            //NOTE: Can you think of ways to stagger how the bullets enter?
            bullets[i].width = 25;
            bullets[i].height = 25;
            bull_rand[i] = false;
        }

        for (int i = 0; i < maxBullets; i++) {
            bullets[i].width = 25;
            bullets[i].height = 25;
            bull_rand[i] = false;
        }


    }

    WorldUpdateResult Update(GameState& game) {

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (currentBoss.type == BOSS_CREATURE_A) {
            numBullets = 5;
        }

        if (currentBoss.type == BOSS_CREATURE_B) {
            numBullets = 10;
        }
        
        if (currentBoss.type == BOSS_CREATURE_C) {
            numBullets = 15;
        }

        for (int i = 0; i < numBullets; i++) {
            bullets[i].y += 3;
            if (bullets[i].y >= 590) {
                bullets[i].x = std::rand() % 801 + 0;
                bullets[i].y = 20;
            }
            if (bull_rand[i]) {
                //NOTE: try also setting y back to start position, see if that makes more sense
                bullets[i].x = std::rand() % 801 + 0;
                game.health -= 10;
                bull_rand[i] = false;

            }
        }

        

        if (spawn_rand) {
            currentBoss.health -= 50;
            game.score = game.score + 50;
        }

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            game.score = game.score + 100;
            if (!moreBosses) {
                world_complete = true;
                game.score = game.score + 100;
            }    
        }

        if (spawn_rand) {
            rand_pos.x = std::rand() % 801 + 0;
            rand_pos.y = std::rand() % 401 + 200;
            spawn_rand = false;
        }
        

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            circ_pos.x--;
            circ_pos.x--;
            circ_pos.x--;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            circ_pos.x++;
            circ_pos.x++;
            circ_pos.x++;
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            circ_pos.y--;
            circ_pos.y--;
            circ_pos.y--;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            circ_pos.y++;
            circ_pos.y++;
            circ_pos.y++;
        }

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
                
        DrawText("World 1 - Enter the circle to damage the bosses", text_x, text_y, 20, WHITE);
        //draw player
        DrawCircleV(circ_pos, 10, BLUE);
        
        //draw bullets
        for (int i = 0; i < numBullets; i++) {
            DrawRectangleRec(bullets[i], ORANGE);
        }
        
        if (draw_spot) {
            DrawCircleV(rand_pos, 20, RED);
        }
        if (CheckCollisionCircles(circ_pos, 10, rand_pos, 20)) {
            spawn_rand = true;
        }
        for (int i = 0; i < numBullets; i++) {
            if (CheckCollisionCircleRec(circ_pos, 10, bullets[i])) {
                bull_rand[i] = true;
            }
        }
        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale; //this is temporary and only works for first boss maybe

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
    }
}