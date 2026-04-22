#include "world1.h"

namespace World1 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;
    int circ_x = 300;
    int circ_y = 200;

    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;
        circ_x = 200;
        circ_y = 200;
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
        DrawText("Template World - Press SPACE to finish", text_x, text_y, 20, WHITE);
        DrawCircle(circ_x, circ_y, 20, BLUE);
    }
}