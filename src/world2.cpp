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
    Rectangle aRadius;
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
    float pushSpeed;
    bool isWallSliding;
    float airDrag;

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

    //Boss stats
    float ePos_x;
    float ePos_y;
    int eSize;
    Rectangle Boss;

    struct minion {
        float posX;
        float posY;
        float sizeX;
        float sizeY;
        Rectangle rec;
        Rectangle Healthbar;
        bool isAlive;
        float minSpeed;
    };
    minion min1;
    minion min2;

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

        //Boss
        ePos_x = 300;
        ePos_y = -500;
        eSize = 1;

        //Minions
       

        min1 = { 250.0f, 350.0f, 50.0f, 50.0f, };
        min1.rec = { min1.posX, min1.posY, min1.sizeX, min1.sizeY };
        min1.Healthbar = { min1.posX, min1.posY + 60, min1.sizeX, min1.sizeY - 40 };
        min1.isAlive = true;
        min1.minSpeed = 2.5;

        min2 = { 350.0f, 250.0f, 50.0f, 50.0f, };
        min2.rec = { min2.posX, min2.posY, min2.sizeX, min2.sizeY };
        min2.Healthbar = { min2.posX, min2.posY + 60, min2.sizeX, min2.sizeY - 40 };
        min2.isAlive = true;
        min2.minSpeed = 2.5;
        


        //Jumping logic
        gravity = 0.5f;
        jumpForce = 12.0f;
        wallPushForce = 300.0f;
        airDrag = 300.0;
        pushSpeed = 0.05;
        wallTime = 2.0f;
        stickTime = 3.0f;
        wallSlideGravity = 50.0f;
        jumpCount = 2;
        maxJumps = 2;
        isWallSliding = false;

        //wallJumpX = 10.0f;
        //walljumpY = -8.0f;

        //Wall and floor shapes
        lWall = { 0 , -450, 40, 1000 };
        rWall = { 790, -450, 40, 1000 };
        floor = { 0, 440, 800, 40 };


        //Rectangle LWallSize = { 100, 5000 };




        //if ((CheckCollisionRecs(, LWallSize))){
            //onLeftwall = true;
            //pSpeed = 0;



    }
    //player info

    WorldUpdateResult Update(GameState& game) {
        //  camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };
        camera.target.y = pPos.y;
        game.score++;
        pVel.y += gravity;
        pVel.x *= (1.0 - airDrag * GetFrameTime());

        touchingLeft = CheckCollisionRecs(pRec, lWall);
        touchingRight = CheckCollisionRecs(pRec, rWall);

        //MOVEMENT
        /*if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pPos.y -= 2.0f * pSpeed;
            pAttackPos.y -= 2 * pSpeed;
        }*/
        if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && !touchingLeft) {
            pPos.x -= 2.0f * pSpeed;
            pAttackPos.x -= 2 * pSpeed;
        }

        /*if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            pPos.y += 2.0f * pSpeed;
            pAttackPos.y += 2 * pSpeed;
        }*/
        if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && !touchingRight) {
            pPos.x += +2.0f * pSpeed;
            pAttackPos.x += 2 * pSpeed;
        }


        //Adding velocity to position
        pPos.x += pVel.x;
        pPos.y += pVel.y;
        pAttackPos.x += pVel.x;
        pAttackPos.y += pVel.y;
        pRec.x = pPos.x;
        pRec.y = pPos.y;



        Boss = { ePos_x, ePos_y, 40, 40 };
        aRadius = { pAttackPos.x, pAttackPos.y, 225, 225 };

        BossState& currentBoss = Bosses::ActiveBoss(game);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionRecs(aRadius, Boss)) {
            pRadiusVis = 0.3f;
            currentBoss.health -= 10;
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pRadiusVis = 0.3f;
        else pRadiusVis = -0.0f;

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }
        //minion collision and death
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (CheckCollisionRecs(aRadius, min1.rec) || CheckCollisionRecs(aRadius, min2.rec))) {
            pRadiusVis = 0.3f;
            if (CheckCollisionRecs(aRadius, min1.rec)) min1.Healthbar.width -= 10;
            if (CheckCollisionRecs(aRadius, min2.rec)) min2.Healthbar.width -= 10;
        }

        if (min1.Healthbar.width <= 0) {
            min1.isAlive = false;
        }
        if (min2.Healthbar.width <= 0) {
            min2.isAlive = false;
        }

        
        //minion movement
        if (min1.posX >= 750 || min1.posX <= 40) {
            min1.minSpeed /= -1;
        }
        if (min2.posX >= 750 || min2.posX <= 40) {
            min2.minSpeed /= -1;
        }
        min1.posX += min1.minSpeed;
        min2.posX += min2.minSpeed;
        min1.rec.x = min1.posX;
        min2.rec.x = min2.posX;
        min1.Healthbar.x = min1.posX;
        min2.Healthbar.x = min2.posX;


        //MOVEMENT
        /*if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pPos.y -= 2.0f * pSpeed;
            pAttackPos.y -= 2 * pSpeed;
        }*/
        if ( !touchingLeft && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) ) ) {
            pPos.x -= 2.0f * pSpeed;
            pAttackPos.x -= 2 * pSpeed;

        }
        else {
            pVel.x = 0;
        }

            if (pVel.x < 0) {
                pVel.x += airDrag;
            }
            else {
                pVel.x = 0;
            }




            if (touchingLeft || touchingRight)
            {

                gravity = 0.2;
                airDrag = 0.70;
                DrawText("Touching Wall", 100, 80, 20, WHITE);
            }

            if (touchingLeft && IsKeyPressed(KEY_SPACE))
            {
                pVel.y = jumpForce += GetFrameTime();
                pVel.x = wallPushForce;
            }
            else if (touchingRight && IsKeyPressed(KEY_SPACE))
            {
                pVel.y = jumpForce += GetFrameTime();
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


            if (!onFloor && (touchingLeft || touchingRight))
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

            if (min1.isAlive) {
                DrawRectangleRec(min1.rec, PURPLE);
                DrawRectangleRec(min1.Healthbar, RED);
            }
            if (min2.isAlive) {
                DrawRectangleRec(min2.rec, PURPLE);
                DrawRectangleRec(min2.Healthbar, RED);
            }


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