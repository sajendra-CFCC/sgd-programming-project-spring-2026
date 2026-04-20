#include "world5.h"

namespace World5 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;

    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        if (IsKeyPressed(KEY_SPACE)) {
            world_complete = true;
        }

        if (IsKeyDown(KEY_LEFT))
            text_x--;
        if (IsKeyDown(KEY_RIGHT))
            text_x++;
        if (IsKeyDown(KEY_UP))
            text_y--;
        if (IsKeyDown(KEY_DOWN))
            text_y++;

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw() {
        DrawText("Template World - Press SPACE to finish", text_x, text_y, 20, WHITE);
    }
}