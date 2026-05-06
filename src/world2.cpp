#include "world2.h"
#include "bosses.h"
#include "shared.h"
#include <cstdlib> //for rand
using namespace std;

namespace World2 {
    bool world_complete = false;
    int text_x = 100;
    int text_y = 100;

    Camera2D camera = { 0 };
    
    Texture2D bg_texture;
    //helper function definition
    void DrawBGTexture(Camera2D camera);
   
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
    int currentHealth;

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
    int eAtk;
    int eSpeed;

    Rectangle celing;

    Rectangle Boss;

    struct minion {
        float posX;
        float posY;
        float sizeX;
        float sizeY;
        Rectangle rec;
        Rectangle Healthbar;
        bool isAlive;
        bool attacking;
        float minSpeed;
        int eAtkSpeed;
        float eAttackTime;
    };
    minion min1;
    minion min2;
    int minionCount;

    void Init() {
        world_complete = false;
        text_x = 100;
        text_y = 100;

        bg_texture = LoadTexture("assets/images/cloudfull.png");

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
        ePos_x = 375;
        ePos_y = -1800;
        eSize = 1;
        eAtk = 10;
        eSpeed = 2.5;

        //Minions
        min1 = { 250.0f, -500.0f, 50.0f, 50.0f, };
        min1.rec = { min1.posX, min1.posY, min1.sizeX, min1.sizeY };
        min1.Healthbar = { min1.posX, min1.posY + 60, min1.sizeX, min1.sizeY - 40 };
        min1.isAlive = true;
        min1.minSpeed = 2.5;

        min2 = { 350.0f, -1000.0f, 50.0f, 50.0f, };
        min2.rec = { min2.posX, min2.posY, min2.sizeX, min2.sizeY };
        min2.Healthbar = { min2.posX, min2.posY + 60, min2.sizeX, min2.sizeY - 40 };
        min2.isAlive = true;
        min2.minSpeed = 2.5;
        min1.attacking = false;
        minionCount = 2;
        min1.eAttackTime = 2.5f;
        min1.eAtkSpeed = 2.0f;
        
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
        lWall = { 0 , -2000, 40, 2550 };
        rWall = { 790, -2000, 40, 2550 };
        floor = { 0, 440, 900, 400 };
        celing = { 0, -2350, 900, 400 };


        //Rectangle LWallSize = { 100, 5000 };

        //if ((CheckCollisionRecs(, LWallSize))){
            //onLeftwall = true;
            //pSpeed = 0;



    }
    //player info

    WorldUpdateResult Update(GameState& game) {
        //  camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };

        float dt = GetFrameTime();
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
            if (minionCount <= 0) currentBoss.health -= 10;
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

        if (!min1.isAlive) minionCount -= 1;
        if (!min2.isAlive) minionCount -= 1;


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

        //boss movement
        if (ePos_x >= 750 || ePos_x <= 40) {
            eSpeed /= -1;
        }
        ePos_x += eSpeed;
       

        //Minion attack logic

        if (min1.eAttackTime > 0) min1.eAttackTime -= dt;
        if (CheckCollisionRecs(pRec, min1.rec) && min1.eAttackTime <= 0)
        {
            if (min1.eAttackTime <= 0 && min1.isAlive == true)
            {
                game.health = game.health - eAtk;
                min1.eAttackTime = min1.eAtkSpeed;
                DrawText("Hit!", 200, 80, 20, WHITE);
                TraceLog(LOG_INFO, "Hitting.");
            }
            else
            {
                min1.eAttackTime = min1.eAtkSpeed;
            }
        }

        if (min1.eAttackTime > 0) min1.eAttackTime -= dt;
        if (CheckCollisionRecs(pRec, min2.rec) && min1.eAttackTime <= 0)
        {
            if (min1.eAttackTime <= 0 && min2.isAlive == true)
            {
                game.health = game.health - eAtk;
                min1.eAttackTime = min1.eAtkSpeed;
                DrawText("Hit!", 200, 80, 20, WHITE);
                TraceLog(LOG_INFO, "Hitting.");
            }
            else
            {
                min1.eAttackTime = min1.eAtkSpeed;
            }
        }

        //MOVEMENT
        /*if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pPos.y -= 2.0f * pSpeed;
            pAttackPos.y -= 2 * pSpeed;
        }*/
        if (!touchingLeft && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) {
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

        bool onCeling = CheckCollisionRecs(pRec, celing);
        if (onCeling)
        {
            pVel.y = 0;
            pPos.y = celing.y - pRec.height;
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

        //draw background image with parralax effect
        DrawBGTexture(camera); //uncomment to put in

        //DrawRectangleV(LWallPos, LWallSize, GRAY);
        DrawRectangleRec(pRec, GREEN);
        DrawRectangleRec(lWall, BLUE);
        DrawRectangleRec(rWall, BLUE);
        DrawRectangleRec(floor, BLUE);
        DrawRectangleRec(celing, BLUE);

            if (min1.isAlive) {
                DrawRectangleRec(min1.rec, PURPLE);
                DrawRectangleRec(min1.Healthbar, RED);
            }
            if (min2.isAlive) {
                DrawRectangleRec(min2.rec, PURPLE);
                DrawRectangleRec(min2.Healthbar, RED);
            }


        DrawText("Defeat all of the minion before fighting to boss!", (float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 + 20, 20, WHITE);
        DrawText("Hit space while on a wall to wall jump!", (float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2, 20, WHITE);
        DrawText("Move Left and right when on the floor!", (float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 - 20, 20, WHITE);
        DrawText("How'd you get up here??? CHEATER.", 200, -2400, 20, RED);

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

    void DrawBGTexture(Camera2D camera) {
        // A. Clear to the sky color present in the image (Deep Navy)
        ClearBackground( Color{ 10, 12, 30, 255 });
        
        float bgScrollFactor = 0.8f; //scroll speed
        //float anchorPos = 670 * bgScrollFactor;
        float anchorPos = 470;
        // B. Calculate the background's vertical position based on camera
        // We anchor the *bottom* of the image to the floor line (440).
        // As the camera Y goes negative (climbing), this position shifts down slowly.
        float textureAnchorY = anchorPos - bg_texture.height;
        //float textureAnchorY = 550 - bg_texture.height; // Anchor to floor
        float parallaxOffset = camera.target.y * bgScrollFactor;
        Vector2 bgPos = { 40.0f, textureAnchorY + parallaxOffset };
        
        // C. Draw the texture in World Space 
        DrawTextureV(bg_texture, bgPos, WHITE);

    }

}