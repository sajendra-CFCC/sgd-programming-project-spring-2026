#include "world5.h"
#include "bosses.h"
#include <iostream>
// for whatever reason main.cpp has an error on the #include "raygui.h" on my system but it works fine anyway. Thank you VSCode, very cool!
namespace World5 {
    bool world_complete = false;
    bool Debug = false; // for testing purposes toggle with Q key
    int WasHitThisFrame = 0;
    int PlayerX = 400;
    int PlayerY = 300;
    int FireCD = 120;
    float BossAccel = 0.25;
    float BossAngletoPlayer = 0.0f;
    Vector2 MouseLoc = {
        0,
        0
    };
    Vector2 BossPos = {
        50,
        500
    };
    Rectangle PlayerHitbox = { MouseLoc.x - 10, MouseLoc.y - 10, 20, 20 };
    Rectangle BossHitbox = { BossPos.x - 15, BossPos.y - 15, 30, 30 };
    typedef struct ProjectileBoss {
        Vector2 Position;
        Vector2 Speed;
    } ProjectileBoss;
    
    //if you put file paths here it might break for assets.. main has to do work first
    //Texture2D TestDirTexture;

    void Init() {
        world_complete = false;
        PlayerX = 400;
        PlayerY = 300;
        //use init to set the value, or we might not be in right directory
        //Texture2D TestDirTexture = LoadTexture("assets/images/TestDir.png");
    }
    void BossFireProj(int count, int delaypershot, int speed) {
        for (int i = 0; i < count; i++) {
            std::cout << "Attempted to fire proj" << std::endl;
            // play shoot audio
        }
    }
    WorldUpdateResult Update(GameState& game) {

        if (game.health <= 0) {
            game.health = 100;
            game.score = 0;
        }

        game.score++;

        if (IsKeyPressed(KEY_ESCAPE)) {
            world_complete = true;
        }
        if (IsKeyPressed(KEY_Q)) {
            Debug = !Debug;
        }
        MouseLoc = GetMousePosition();
        Rectangle PlayerHitbox = { MouseLoc.x - 10, MouseLoc.y - 10, 20, 20 };
        if (CheckCollisionRecs(PlayerHitbox, BossHitbox)) {
            game.health -= 1;
            if (Debug == true) {
            DrawText(TextFormat("Hit Detected"), MouseLoc.x - 55, MouseLoc.y - 30, 20, RED);
        }
        }
        if (game.health <= 0) {
            world_complete = true;
        }
        if (rand() % 3 == 0 && FireCD <= 0) {
            if (rand() % 1 == 0){
                BossFireProj(rand() % 6 + 1, rand() % 1, rand() % 3 + 1);
            } else {
                BossFireProj(rand() % 3 + 1, rand() % 1, rand() % 3 + 1);
            }
            FireCD = rand() % (120 - 40 + 1) + 40;
        } else {
            FireCD--;
        }

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (world_complete) {
            return WORLD_COMPLETED;
        } else {
            return WORLD_IN_PROGRESS;
        }
    }

    void Draw( const GameState& game) {
        DrawText(TextFormat("Press ESC to finish / Use mouse to move"), 369, 40, 20, WHITE);
        DrawText(TextFormat("Survive to gain SCORE"), 564, 70, 20, WHITE);
        MouseLoc = GetMousePosition();
        DrawCircleV(MouseLoc, 20, BLUE);
        Rectangle PlayerHitbox = { MouseLoc.x - 10, MouseLoc.y - 10, 20, 20 };
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        Bosses::Draw(currentBoss, BossPos.x, BossPos.y, 1);
        if (Debug == true) {
            DrawRectangleRec(PlayerHitbox, WHITE); // hitbox visualization
            DrawRectangleRec(BossHitbox, WHITE);
            DrawText(TextFormat("Debug on"), MouseLoc.x - 49, MouseLoc.y + 25, 20, YELLOW);
            DrawLineV(MouseLoc, BossPos, GREEN); // line from player to boss
        }
    }
}