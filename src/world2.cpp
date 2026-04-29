#include "world2.h"
#include "bosses.h"
#include "shared.h"


namespace World2 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;

    Camera2D camera = { 0 };

    //player
    

    float pSpeed;
    Vector2 pPos;
    Rectangle pRec;
    Vector2 pSize;
    Vector2 pAttackRadius;
    Vector2 pAttackPos;
    float pRadiusVis;
    Vector2 pVel;

    Rectangle pRad;

    //Walls
    Vector2 LWallPos;
    Vector2 LWallSize;
    Vector2 RWallPos;
    Vector2 RWallSize;
    Rectangle lWall;
    Rectangle rWall;

    bool touchingLeft;
    bool touchingRight;

    //Jump logic
    float gravity;
    float jumpForce;
    float wallPushForce;
    float wallTime;
    float stickTime;
    float wallSlideGravity;
    int jumpCount;
    int maxJumps;
    bool isWallSliding;
    
   /* bool started = false;
    bool onLeftwall;
    bool onRightwall;
    float wallJumpX;
    float walljumpY;
    float downSlide;
    float gravity;
    bool canJump;
    float jumpSpeed;
    */

    Rectangle floor;







    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;

        //Camera
        camera.offset = { (float)GetScreenWidth() / 2 - 420, (float)GetScreenHeight() / 2 };
        //camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };
        camera.rotation = 0;
        camera.zoom = 1;

        //Player
        pPos = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        pVel = { 0, 0 };
        pRec = { pPos.x, pPos.y, 150, 150 };
        pSpeed = 10;
        pVel = { 0, 0 };
        pAttackPos.x = pPos.x - 35;
        pAttackPos.y = pPos.y - 35;
        pAttackRadius = { 225, 225 };
        pRadiusVis = 0.0f;
        pSize = { 100, 100 };
        pRad = { 100, 100 };


        //Jumping logic
        gravity = 0.5f;
        jumpForce = 15.0f;
        wallPushForce = 663.0f;
        wallTime = 2.0f;
        stickTime = 3.0f;
        wallSlideGravity = 10.0f;
        jumpCount = 2;
        int maxJumps = 2;
        isWallSliding = false;

        //wallJumpX = 10.0f;
        //walljumpY = -8.0f;

        //Wall and floor shapes
        lWall = { 0 , -450, 40, 1000};
        rWall = { 790, -450, 40, 1000 };
        floor = { 0, 440, 800, 40 };


        //Rectangle LWallSize = { 100, 5000 };




        //if ((CheckCollisionRecs(, LWallSize))){
            //onLeftwall = true;
            //pSpeed = 0;



    }
        

    

    struct Minions
    {
        
    };
    //Boss stats
    int ePos_x = 300;
    int ePos_y = 350;
    int eSize = 1;
    Rectangle Boss;
    //Boss{ ePos_x, ePos_y, boss_radius.x, boss_radius.y };

    //player info

    WorldUpdateResult Update(GameState& game) {
      //  camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };
        camera.target.y = pPos.y ;
        game.score++;
        pVel.y += gravity;

        touchingLeft = CheckCollisionRecs(pRec, lWall);
        touchingRight = CheckCollisionRecs(pRec, rWall);

        //Adding velocity to position
        pPos.x += pVel.x;
        pPos.y += pVel.y;
        pAttackPos.x += pVel.x;
        pAttackPos.y += pVel.y;
        pRec.x = pPos.x;
        pRec.y = pPos.y;

        BossState& currentBoss = Bosses::ActiveBoss(game);

        //CheckCollisionRecs();

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }


        //MOVEMENT
        /*if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pPos.y -= 2.0f * pSpeed;
            pAttackPos.y -= 2 * pSpeed;
        }*/
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            pPos.x -= 2.0f * pSpeed;
            pAttackPos.x -= 2 * pSpeed;
        }
        /*if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            pPos.y += 2.0f * pSpeed;
            pAttackPos.y += 2 * pSpeed;
        }*/
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            pPos.x += +2.0f * pSpeed;
            pAttackPos.x += 2 * pSpeed;
        }
        else pVel.x = 0;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            pRadiusVis = 0.3f;
            currentBoss.health -= 10;
        }
        else pRadiusVis = -0.0f;
        



        if (touchingLeft || touchingRight)
        {
            pSpeed = 0;
            gravity = 0.6;
            DrawText("Touching Wall", 100, 80, 20, WHITE);
        }

        if (touchingLeft && IsKeyPressed(KEY_SPACE))
        {
            pVel.y = jumpForce;
            pVel.x = wallPushForce;
        }
        else if (touchingRight && IsKeyPressed(KEY_SPACE))
        {
            pVel.y = jumpForce;
            pVel.x = -wallPushForce;
        }
        else
        {
            isWallSliding = false;
            wallTime = 0;
        }



        bool onFloor = CheckCollisionRecs(pRec, floor);
        if (onFloor) {
            pSpeed = 10;
            pVel.y = 0;
            pPos.y = floor.y - pRec.height;
            pAttackPos.y = floor.y - pRec.height - 35;
            DrawText("On Floor", 100, 100, 20, WHITE);

            //Jumping on floor
            if (IsKeyPressed(KEY_SPACE)) {
                pVel.y = jumpForce;
            }
        }
        else {
            // Only apply gravity if NOT on the floor
            pVel.y += gravity;
        }


        if (touchingLeft || touchingRight && !onFloor)
        {
            isWallSliding = true;
        }

        if (isWallSliding == true)
        {
            wallTime += GetFrameTime();

            if (wallTime < stickTime)
            {
                isWallSliding = false;
            }
            else
            {
                isWallSliding = true;
                pVel.y = wallSlideGravity;

            }
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            if (onFloor || touchingLeft || touchingRight)
            {
                pVel.y = -jumpForce;
                onFloor = false;
                jumpCount = 1;
            }
            else if (jumpCount < maxJumps)
            {
                pVel.y = -jumpForce;
                jumpCount++;
            }
        }

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        //Camera Start
        BeginMode2D(camera);

        //DrawRectangleV(LWallPos, LWallSize, GRAY);
        DrawRectangleRec(pRec, GREEN);
        DrawRectangleRec(lWall, BLUE);
        DrawRectangleRec(rWall, BLUE);
        DrawRectangleRec(floor, BLUE);

        DrawText("Hit space while on a wall to wall jump!", (float)GetScreenWidth() / 2 - 10, (float)GetScreenHeight() / 2, 20, WHITE);
        DrawText("Move Left and right when on the floor!", (float)GetScreenWidth() / 2 - 10, (float)GetScreenHeight() / 2 - 20, 20, WHITE);

        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * eSize;
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, ePos_x, ePos_y, eSize);
        int boss_radius = Bosses::GetHitRadius(currentBoss, eSize);

        Bosses::Draw(currentBoss, ePos_x, ePos_y, eSize);
        Bosses::DrawHealthBar(currentBoss, ePos_x - boss_size, ePos_y + boss_size, boss_size * 2);

        DrawRectangleV(pAttackPos, pAttackRadius, Fade(RED, pRadiusVis));
       // DrawRectangleV(pPos, pSize, GREEN);

        DrawRectangleLinesEx(bossHB, 1, RED);
        //DrawCircleLines(ePos_x, ePos_y, boss_radius, GREEN);
        EndMode2D();
    }
}