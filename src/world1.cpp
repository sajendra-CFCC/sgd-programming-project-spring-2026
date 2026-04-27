#include "world1.h"
#include "bosses.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace World1 {
    bool world_complete;
    bool spawn_rand;
    bool draw_spot;
    bool bull_rand1;
    int text_x;
    int text_y;
    int boss_x;
    int boss_y;
    int boss_scale;
    Vector2 circ_pos;
    Vector2 rand_pos;
    Rectangle bullet_1;

    void Init() {
        world_complete = false;
        spawn_rand = false;
        draw_spot = true;
        bull_rand1 = false;
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
        bullet_1.x = 350;
        bullet_1.y = 20;
        bullet_1.width = 25;
        bullet_1.height = 25;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (spawn_rand) {
            currentBoss.health -= 50;
        }

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }

        if (spawn_rand) {
            rand_pos.x = std::rand() % 801 + 0;
            rand_pos.y = std::rand() % 401 + 200;
            spawn_rand = false;
        }
        if (bull_rand1) {
            bullet_1.x = std::rand() % 801 + 0;
            bull_rand1 = false;
        }

        if (IsKeyDown(KEY_LEFT)) {
            circ_pos.x--;
            circ_pos.x--;
            circ_pos.x--;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            circ_pos.x++;
            circ_pos.x++;
            circ_pos.x++;
        }
        if (IsKeyDown(KEY_UP)) {
            circ_pos.y--;
            circ_pos.y--;
            circ_pos.y--;
        }
        if (IsKeyDown(KEY_DOWN)) {
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
        DrawCircleV(circ_pos, 10, BLUE);
        DrawRectangleRec(bullet_1, ORANGE);
        if (draw_spot) {
            DrawCircleV(rand_pos, 20, RED);
        }
        if (CheckCollisionCircles(circ_pos, 10, rand_pos, 20)) {
            spawn_rand = true;
        }
        if (CheckCollisionCircleRec(circ_pos, 10, bullet_1)) {
            bull_rand1 = true;
        }
        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale; //this is temporary and only works for first boss maybe

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
    }
}