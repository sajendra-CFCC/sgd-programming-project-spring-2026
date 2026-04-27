#include "world3.h"
#include "bosses.h"
#include <iostream>
#include <external/stb_rect_pack.h>
#include <algorithm> // for std::min/std::max


namespace World3 {
    bool world_complete;
    int boss_x;
    int boss_y;
    int boss_scale;
    int player_x;
    int player_y;
    int player_scale;
    //Music bgm;

    const int player_width = 100;
    const int player_height = 25;

    
    // movement parameters
    const int playerSpeed = 1; // pixels per frame

    //ball variables and paramters
    const int ballRadius = 10;
    Vector2 ballPosition;
    Vector2 ballSpeed;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        //InitAudioDevice();
       


        // initial boss placement
        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;

        // initial player placement (center-ish)
        player_x = 100;
        player_y = SCREEN_HEIGHT - 100;
        player_scale = 1;

        //inital ball
        ballPosition.x = player_x;
        ballPosition.y = player_y;

        ballSpeed.x = playerSpeed;
        ballSpeed.y = 100;
    }

    WorldUpdateResult Update(GameState& game) {
        //game.score++; // just updating score every frame for some reason

        
        //float dt = GetFrameTime(); // frame time for smooth movement
        //int move = static_cast<int>(baseSpeed * dt);

        // continuous input for smooth movement
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player_x += playerSpeed;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player_x -= playerSpeed;
        }
        //if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
          //  player_y -= playerSpeed;
        //}
       // if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        //    player_y += playerSpeed;
       // }

        // clamp player to screen bounds (keep whole square visible)
        int player_size = 100 * player_scale;
        int half = player_size / 2;
        player_x = std::max(half, std::min(player_x, SCREEN_WIDTH - half));
        player_y = std::max(half, std::min(player_y, SCREEN_HEIGHT - half));

        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);

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
        // draw background text
        int text_x = 250;
        int text_y = 100;
        DrawText("Evan and Richie World", text_x, text_y, 25, PURPLE);

        // draw player square
        DrawRectangle(player_x, player_y, player_width, player_height, RED);
  

        // get the current boss and draw it
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 75 * boss_scale;
        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);

        //draw ball
        DrawCircleV(ballPosition, ballRadius, BLUE);
    }

}