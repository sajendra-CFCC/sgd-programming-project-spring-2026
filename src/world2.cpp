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
        jumpForce = -10.0f;
        wallPushForce = 50.0f;
        wallTime = 20.0f;
        stickTime = 3.0f;
        wallSlideGravity = 10.0f;
        isWallSliding = false;

        //wallJumpX = 10.0f;
        //walljumpY = -8.0f;

        //Wall and floor shapes
        lWall = { 0 , 0, 40, 400};
        rWall = { 790, 100, 40, 400 };
        floor = { 100, 440, 700, 40 };

        touchingLeft = CheckCollisionRecs(pRec, lWall);
        touchingRight = CheckCollisionRecs(pRec, rWall);
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

    //player info

    WorldUpdateResult Update(GameState& game) {
      //  camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };
        camera.target.y = pPos.y ;
        game.score++;
        pVel.y += gravity;

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

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            pRadiusVis = 0.3f;
        }
        else pRadiusVis = -0.0f;
        

        


       /* bool onFloor = CheckCollisionRecs(pRec, floor);
        if (onFloor) {
            pVel.y = 0;
            pPos.y = floor.y - pRec.height;

            //Jumping on floor
            if (IsKeyPressed(KEY_SPACE)) {
                pVel.y = jumpForce;
            }
        }
        else {
            // Only apply gravity if NOT on the floor
            pVel.y += gravity;
        }
        */

        bool onFloor = CheckCollisionRecs(pRec, floor);
        if (onFloor) {
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
        
        if (touchingLeft || touchingRight)
        {
            DrawText("Touching Wall", 100, 100, 20, WHITE);
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

        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 30 * eSize;
        Rectangle bossHB = Bosses::GetHitbox(currentBoss, ePos_x, ePos_y, eSize);
        int boss_radius = Bosses::GetHitRadius(currentBoss, eSize);

        Bosses::Draw(currentBoss, ePos_x, ePos_y, eSize);
        Bosses::DrawHealthBar(currentBoss, ePos_x - eSize, ePos_y + eSize, eSize * 2);

        DrawRectangleV(pAttackPos, pAttackRadius, Fade(RED, pRadiusVis));
       // DrawRectangleV(pPos, pSize, GREEN);

        DrawRectangleLinesEx(bossHB, 1, RED);
        //DrawCircleLines(ePos_x, ePos_y, boss_radius, GREEN);
        EndMode2D();
    }
}