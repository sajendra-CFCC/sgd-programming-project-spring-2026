#include "world3.h"
#include "bosses.h"
#include <iostream>

namespace World3 {
    bool world_complete;
        
    int boss_x;
    int boss_y;
    int boss_scale;
    Music bgm;

    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;

        //InitAudioDevice();

        //// level background music
        //Music bgm = LoadMusicStream("/assets/mp3/active_camo.mp3");
        

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
        //PlayMusicStream(bgm);

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        //do the drawing for your world here
        int text_x = 250;
        int text_y = 100;
        DrawText("Evan and Richie World", text_x, text_y, 25, WHITE);

        //get the current boss
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * boss_scale;

        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);
        

        /*UnloadMusicStream(bgm);
        CloseAudioDevice();*/
    }

}