#include "world5.h"
// for whatever reason main.cpp has an error on the #include "raygui.h" on my system but it works fine anyway. Thank you VSCode, very cool!
namespace World5 {
    bool world_complete = false;
    int PlayerX = 400;
    int PlayerY = 300;

    void Init() {
        world_complete = false;
        PlayerX = 400;
        PlayerY = 300;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        if (IsKeyPressed(KEY_ESCAPE)) {
            world_complete = true;
        }

        if (IsKeyDown(KEY_LEFT))
            PlayerX--;
        if (IsKeyDown(KEY_RIGHT))
            PlayerX++;
        if (IsKeyDown(KEY_UP))
            PlayerY--;
        if (IsKeyDown(KEY_DOWN))
            PlayerY++;

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw() {
        DrawText(TextFormat("Press ESC to finish / Press ARROWS to move"), 320, 10, 20, WHITE);
        DrawText(TextFormat("Survive to gain SCORE"), 564, 40, 20, WHITE);
        DrawCircle(PlayerX, PlayerY, 20, BLUE);
    }
}