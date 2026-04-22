#include "world1.h"

namespace World1 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;
    int circ_x = SCREEN_WIDTH/2;
    int circ_y = SCREEN_HEIGHT/2;

    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;
        circ_x = SCREEN_WIDTH/2;
        circ_y = SCREEN_HEIGHT/2;
    }

    WorldUpdateResult Update(GameState& game) {
        game.score++;

        if (IsKeyPressed(KEY_SPACE)) {
            world_complete = true;
        }

        if (IsKeyDown(KEY_LEFT))
            circ_x--;
        if (IsKeyDown(KEY_RIGHT))
            circ_x++;
        if (IsKeyDown(KEY_UP))
            circ_y--;
        if (IsKeyDown(KEY_DOWN))
            circ_y++;

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw() {
        DrawText("World 1 - Press SPACE to finish", text_x, text_y, 20, WHITE);
        DrawCircle(circ_x, circ_y, 10, BLUE);
    }
}