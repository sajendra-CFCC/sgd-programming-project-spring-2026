#include "world5.h"
#include "bosses.h"
#include <iostream>
#include <cmath>

namespace World5 {
    bool world_complete = false;
    bool Debug = false; // for testing purposes toggle with Q key
    int PlayerX = 400;
    int PlayerY = 300;
    int FireCD = 120;
    int BossRad = 30;
    int PlayerRad = 20;
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
    Rectangle PlayerHitbox = { MouseLoc.x - (PlayerRad/2), MouseLoc.y - (PlayerRad/2), (PlayerRad), (PlayerRad) };
    Rectangle BossHitbox = { BossPos.x - (BossRad/2), BossPos.y - (BossRad/2), (BossRad), (BossRad) };
    typedef struct ProjectileBoss {
        Vector2 Position;
        Vector2 Speed;
        float Angle;
        int Lifetime;
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
    
    //what does each variable mean? use comments if not clear
    // count is meant to be the number of projectiles fired, delaypershot is the delay between each projectile (technically in frames but its *60 so the input is in seconds), and speed is the speed of the projectiles.
    void BossFireProj(int count, float delaypershot, int speed) { //Be careful doing delay like this, it will hold up the whole program
        for (int i = 0; i < count; i++) {
            /*for (int d = 0; d < delaypershot * 60; d++) {
                std::cout << "Waiting" << std::endl;
            }*/
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

        Rectangle PlayerHitbox = { MouseLoc.x - (PlayerRad/2), MouseLoc.y - (PlayerRad/2), (PlayerRad), (PlayerRad) }; //update hitboxes each frame
        Rectangle BossHitbox = { BossPos.x - (BossRad/2), BossPos.y - (BossRad/2), (BossRad), (BossRad) };

        if (CheckCollisionRecs(PlayerHitbox, BossHitbox)) {
            game.health -= 1;
            if (Debug == true) {
            DrawText(TextFormat("Hit Detected"), MouseLoc.x - 55, MouseLoc.y - 30, 20, RED);
        }
        }

        if (game.health <= 0) {
            world_complete = true;
        }

        //this bit of code is not clear, nor what you are trying to accomplish. Use comments if not clear
        //check random functions exposed by raylib, they can be easier to use
        // this is meant to randomly attempt to fire projectiles each frame, with FireCD working as a cooldown between each trigger
        if (rand() % 3 == 0 && FireCD <= 0) {
            if (rand() % 2 == 0){
                BossFireProj(rand() % 6 + 1, 1.0f, rand() % 3 + 1);
            } else { // this bit is meant to either fire a larger or smaller number of projectiles
                BossFireProj(rand() % 3 + 1, 1.0f, rand() % 3 + 1);
            }
            FireCD = rand() % (120 - 40 + 1) + 40;
            BossAccel = 1.0f;
        } else {
            FireCD--;
        }

        BossState& currentBoss = Bosses::ActiveBoss(game);


        BossAngletoPlayer = atan2(MouseLoc.y - BossPos.y, MouseLoc.x - BossPos.x); // gets the angle from the boss to the player to be used for moving the boss
        BossPos.x += cos(BossAngletoPlayer) * BossAccel; // move the boss
        BossPos.y += sin(BossAngletoPlayer) * BossAccel;


        BossAccel += 0.04f; //boss speeds up over time

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
        DrawCircleV(MouseLoc, PlayerRad, BLUE);
        Rectangle PlayerHitbox = { MouseLoc.x - (PlayerRad/2), MouseLoc.y - (PlayerRad/2), (PlayerRad), (PlayerRad) };
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        Bosses::Draw(currentBoss, BossPos.x, BossPos.y, 1);
        if (Debug == true) {
            DrawRectangleRec(PlayerHitbox, WHITE); // hitbox visualization
            DrawRectangle(BossPos.x - (BossRad/2), BossPos.y - (BossRad/2), (BossRad), (BossRad), WHITE); // changed cause old method didnt want to follow the boss after it started moving
            DrawText(TextFormat("Debug on"), MouseLoc.x - 49, MouseLoc.y + 25, 20, YELLOW);
            DrawLineV(MouseLoc, BossPos, GREEN); // line from player to boss
        }
    }
}