#include "overworld.h"

//return -1 for stay on overworld MENU
//return number >= 0 for entering a world
// return zero for world zero (template)
//return 1 for entering world one
// etc..
int UpdateOverworld(GameState &game) {
    if( IsKeyPressed(KEY_ZERO) ) {
        return 0;
    }
    if( IsKeyPressed(KEY_ONE) ) {
        return 1;
    }
    if( IsKeyPressed(KEY_TWO) ) {
        return 2;
    }
   /* if( IsKeyPressed(KEY_THREE) ) {
        return 3;
    }
    if (IsKeyPressed(KEY_FOUR)) {
        return 4;
    }*/
    if (IsKeyPressed(KEY_FIVE)) {
        return 5;
    }
    return -1; //stay on the MENU
}

void DrawOverworld() {
    DrawText("CLASS ARCADE BOSS DUNGEON", 150, 100, 30, WHITE);
    DrawText("Press 0 - World 0", 200, 200, 20, YELLOW);
    DrawText("Press 1 - World 1", 200, 220, 20, YELLOW);
    DrawText("Press 2 - World 2", 200, 240, 20, YELLOW);
    DrawText("Press 3 - World 3", 200, 260, 20, YELLOW);
    DrawText("Press 4 - World 4", 200, 280, 20, YELLOW);
    DrawText("Press 5 - World 5", 200, 300, 20, DARKPURPLE);
    DrawText("ESC - Quit", 200, 400, 20, RAYWHITE);
}
