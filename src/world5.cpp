#include "world5.h"
#include "bosses.h"
// for whatever reason main.cpp has an error on the #include "raygui.h" on my system but it works fine anyway. Thank you VSCode, very cool!
namespace World5 {
    bool world_complete = false;
    int PlayerX = 400;
    int PlayerY = 300;
    Vector2 MouseLoc = {
        0,
        0
    };
    
    //if you put file paths here it might break for assets.. main has to do work first
    Texture2D TestDirTexture;

    void Init() {
        world_complete = false;
        PlayerX = 400;
        PlayerY = 300;
        //use init to set the value, or we might not be in right directory
        Texture2D TestDirTexture = LoadTexture("assets/images/TestDir.png");
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        if (IsKeyPressed(KEY_ESCAPE)) {
            world_complete = true;
        }

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses) {
                world_complete = true;
            }
        }

        if (world_complete) {
            return WORLD_COMPLETED;
        } else {
            return WORLD_IN_PROGRESS;
        }
    }

    void Draw() {
        DrawText(TextFormat("Press ESC to finish / Use mouse to move"), 369, 40, 20, WHITE);
        DrawText(TextFormat("Survive to gain SCORE"), 564, 70, 20, WHITE);
        MouseLoc = GetMousePosition(); //why
        DrawRectangleV({MouseLoc.x - 10, MouseLoc.y - 10}, {20, 20}, RED);
        DrawCircleV(MouseLoc, 20, BLUE);
        DrawTextureV(TestDirTexture, {0,0}, WHITE);
    }
}