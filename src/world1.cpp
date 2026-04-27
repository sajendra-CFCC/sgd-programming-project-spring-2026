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
    int circ_x;
    int circ_y;
    int boss_x;
    int boss_y;
    int boss_scale;
    int rand_numx;
    int rand_numy;

    void Init() {
        world_complete = false;
        spawn_rand = false;
        draw_spot = true;
        text_x = 145;
        text_y = 550;
        circ_x = SCREEN_WIDTH/2;
        circ_y = SCREEN_HEIGHT/2;
        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2 - 200;
        boss_scale = 1;
        std::srand(std::time(0));
        rand_numx = SCREEN_WIDTH / 2;
        rand_numy = SCREEN_HEIGHT / 2 + 200;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (IsKeyPressed(KEY_E)) {
            currentBoss.health -= 10;
        }

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }

        if (spawn_rand) {
            rand_numx = std::rand() % 801 + 0;
            rand_numy = std::rand() % 601 + 0;
            spawn_rand = false;
        }

        if (IsKeyDown(KEY_LEFT)) {
            circ_x--;
            circ_x--;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            circ_x++;
            circ_x++;
        }
        if (IsKeyDown(KEY_UP)) {
            circ_y--;
            circ_y--;
        }
        if (IsKeyDown(KEY_DOWN)) {
            circ_y++;
            circ_y++;
        }

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
                
        DrawText("World 1 - Enter the shapes to damage the bosses", text_x, text_y, 20, WHITE);
        DrawCircle(circ_x, circ_y, 10, BLUE);
        if (draw_spot) {
            DrawCircle(rand_numx, rand_numy, 10, RED);
            draw_spot = false;
        }
        
        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale; //this is temporary and only works for first boss maybe

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
    }
}