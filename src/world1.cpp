#include "world1.h"
#include "bosses.h"
#include <iostream>

namespace World1 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;
    int circ_x = SCREEN_WIDTH/2;
    int circ_y = SCREEN_HEIGHT/2;
    int boss_x;
    int boss_y;
    int boss_scale;

    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;
        circ_x = SCREEN_WIDTH/2;
        circ_y = SCREEN_HEIGHT/2;
        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2 - 200;
        boss_scale = 1;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (IsKeyPressed(KEY_SPACE)) {
            currentBoss.health -= 10;
        }

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            world_complete = true;
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
                
        DrawText("World 1 - Press SPACE to finish", text_x, text_y, 20, WHITE);
        DrawCircle(circ_x, circ_y, 10, BLUE);
        
        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale; //this is temporary and only works for first boss maybe
        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
    }
}