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
            onLeftwall = true;
            pSpeed = 0;
    }
        

    

    struct Minions
    {
        
    };
    //Boss stats
    Vector2 eSize = { 100, 100};
    Vector2 ePos = { 75, 150 };

    //player info

    WorldUpdateResult Update(GameState& game) {
        
        game.score++;


        if (IsKeyDown(KEY_W)) pPos.y -= 2.0f * pSpeed;
        if (IsKeyDown(KEY_A)) pPos.x -= 2.0f * pSpeed;
        if (IsKeyDown(KEY_S)) pPos.y += 2.0f * pSpeed;
        if (IsKeyDown(KEY_D)) pPos.x += +2.0f * pSpeed;

        //if (IsKeyPressed(KEY_SPACE)) 

        camera.target = { pPos.x + pSpeed, pPos.y + pSpeed };

        if (IsKeyPressed(KEY_SPACE)) {
            world_complete = true;
        }

        if (IsKeyDown(KEY_LEFT))
            text_x--;
        if (IsKeyDown(KEY_RIGHT))
            text_x++;
        if (IsKeyDown(KEY_UP))
            text_y--;
        if (IsKeyDown(KEY_DOWN))
            text_y++;

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        //Camera Start
        BeginMode2D(camera);

        DrawRectangleV(LWallPos, LWallSize, GRAY);


        DrawRectangleV( ePos, eSize, RED);
        DrawRectangleV(pPos, pSize, GREEN);

        DrawText("Template World - Press SPACE to finish", text_x, text_y, 20, WHITE);
    }
}