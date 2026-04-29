#include "world2.h"
#include "bosses.h"
#include "shared.h"


namespace World2 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;

    //player
    

    float pSpeed;
    Vector2 pPos;
    Vector2 pSize;
    Vector2 pAttackRadius;
    Vector2 pAttackPos;

    //Walls
    Vector2 LWallPos;
    Vector2 LWallSize;
    Vector2 RWallPos;
    Vector2 RWallSize;


    //Jump logic
    bool started = false;
    bool onLeftwall;
    bool onRightwall;
    float wallJumpX;
    float walljumpY;
    float downSlide;
    float gravity;
    bool canJump;
    float jumpSpeed;
    

    Camera2D camera = { 0 };


    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;

        LWallPos = { 0, 255 };
        Rectangle LWallSize = { 100, 5000 };

        //Player
        pPos = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        pAttackPos.x = pPos.x - 25;
        pAttackPos.y = pPos.y - 25;
        pAttackRadius = { 150, 150 };
        pSize = { 100, 100 };
        Rectangle pRad = { 100, 100 };
        pSpeed = 10;
        wallJumpX = 10.0f;
        walljumpY = -8.0f;
        gravity = 1.0f;

        camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        camera.rotation = 0;
        camera.zoom = 1;

        //if ((CheckCollisionRecs(, LWallSize))){
            //onLeftwall = true;
            //pSpeed = 0;
    }
        

    

    struct Minions
    {
        
    };
    //Boss stats
    int ePos_x = 75;
    int ePos_y = 100;
    int eSize = 1;

    //player info

    WorldUpdateResult Update(GameState& game) {
        
        game.score++;

        BossState& currentBoss = Bosses::ActiveBoss(game);

        //CheckCollisionRecs();

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pPos.y -= 2.0f * pSpeed;
            pAttackPos.y -= 2 * pSpeed;
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            pPos.x -= 2.0f * pSpeed;
            pAttackPos.x -= 2 * pSpeed;
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            pPos.y += 2.0f * pSpeed;
            pAttackPos.y += 2 * pSpeed;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            pPos.x += +2.0f * pSpeed;
            pAttackPos.x += 2 * pSpeed;
        }

        //if (IsKeyPressed(KEY_SPACE)) 

        camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        //Camera Start
        BeginMode2D(camera);

        DrawRectangleV(LWallPos, LWallSize, GRAY);

        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * eSize;
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, ePos_x, ePos_y, eSize);
        int boss_radius = Bosses::GetHitRadius(currentBoss, eSize);

        Bosses::Draw(currentBoss, ePos_x, ePos_y, eSize);
        Bosses::DrawHealthBar(currentBoss, ePos_x - eSize, ePos_y + eSize, eSize * 2);

        DrawRectangleV(pAttackPos, pAttackRadius,Fade(RED, 0.3f));
        DrawRectangleV(pPos, pSize, GREEN);

        DrawRectangleLinesEx(bossHB, 1, RED);
        //DrawCircleLines(ePos_x, ePos_y, boss_radius, GREEN);
        EndMode2D();
    }
}