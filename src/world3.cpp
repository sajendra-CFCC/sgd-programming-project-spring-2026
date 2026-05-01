#include "world3.h"
#include "bosses.h"
#include "raymath.h"
#include <iostream>



namespace World3 {
    //helper function for rotating point about center, will implement later
    Vector2 RotatePointAroundCenter(Vector2 point, float angleDegrees);


    bool world_complete;
    int boss_x;
    int boss_y;
    int boss_scale;
    int player_x;
    int player_y;
    int player_rotation;
    
    
    const float player_width = 150;
    const float player_height = 25;

    
    // movement parameters
    const int playerSpeed = 5; // pixels per frame

    //ball variables and paramters
    const int initialBallSpeed = 3;
    const int ballRadius = 10;
    Vector2 ballPosition;
    Vector2 ballSpeed;

    const int CENTER_X = SCREEN_WIDTH / 2;
    const int CENTER_Y = SCREEN_HEIGHT / 2;
    //player circle positions
    const int CIRC_RADIUS = 275;
    const Vector2 PLAYER_START_POINT = { CENTER_X, CENTER_Y - CIRC_RADIUS };
    Vector2 player_point_on_circle;

    //I'm going to suggest you check collision with line on "face" of rotated rectangle
    Vector2 COLLISION_LINE_START_POINT1 = { PLAYER_START_POINT.x - player_width/2, PLAYER_START_POINT.y + player_height/2 };
    Vector2 COLLISION_LINE_START_POINT2 = { PLAYER_START_POINT.x + player_width/2, PLAYER_START_POINT.y + player_height/2};
    Vector2 collsionLinePoint1;
    Vector2 collsionLinePoint2;


    void Init() {
        //set up anything you need for your game / world here
        world_complete = false;
        
        // initial boss placement
        boss_x = SCREEN_WIDTH / 2;
        boss_y = SCREEN_HEIGHT / 2;
        boss_scale = 1;

        // initial player placement (center-ish)
        player_x = 100;
        player_y = SCREEN_HEIGHT - 100;
        

        //inital ball
        ballPosition.x = player_x;
        ballPosition.y = player_y;

        ballSpeed.x = initialBallSpeed;
        ballSpeed.y = -initialBallSpeed;

        ballPosition.x = 100;
        ballPosition.y = 100;
        ballSpeed.x = -2;
        ballSpeed.y = -3;

        player_rotation = 0;
        player_point_on_circle = RotatePointAroundCenter(PLAYER_START_POINT, player_rotation);
        collsionLinePoint1 = RotatePointAroundCenter(COLLISION_LINE_START_POINT1, player_rotation);
        collsionLinePoint2 = RotatePointAroundCenter(COLLISION_LINE_START_POINT2, player_rotation);
    }

    WorldUpdateResult Update(GameState& game) {
        // continuous input for smooth movement
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
           player_x += playerSpeed;
           player_rotation++;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player_x -= playerSpeed;
            player_rotation--;
        }
        //if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
          //  player_y -= playerSpeed;
        //}
       // if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        //    player_y += playerSpeed;
       // }

        //update position ball
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        //check bounds
        if (ballPosition.y <= 0)
        {
            ballSpeed.y *= -1;
        }

        if (ballPosition.x <= 0) 
        {
            ballSpeed.x *= -1;
        }

        if (ballPosition.y >= 600)
        {
            ballSpeed.y *= -1;
        }

        if (ballPosition.x >= 800)
        {
            ballSpeed.x *= -1;
        }

        //keep paddle position on circle updated
        player_point_on_circle = RotatePointAroundCenter(PLAYER_START_POINT, player_rotation);
        //keep collision line rotated
        collsionLinePoint1 = RotatePointAroundCenter(COLLISION_LINE_START_POINT1, player_rotation);
        collsionLinePoint2 = RotatePointAroundCenter(COLLISION_LINE_START_POINT2, player_rotation);
        

        //Ball and Rectangle Collision Setup
        Rectangle playerRect = { (float)player_x, (float)player_y, player_width, player_height };
       /* if (CheckCollisionCircleRec(ballPosition, ballRadius, playerRect)) {
            std::cout << "COLLIDE\n";
            ballSpeed.y *= -1;
            ballSpeed.x = GetRandomValue(-5, 5);
        }*/



        //get the current boss
        BossState& currentBoss  = Bosses::ActiveBoss(game);

        //NOTE this is incorrect (Rectangle takes for paramters
        Rectangle BossBox = { (int)boss_x, (int)boss_y, boss_scale };
        //use the built in get boss hitbox , will return rectangle for you
        if (CheckCollisionCircleRec(ballPosition, ballRadius, BossBox)) {
            std::cout << "HIT!\n";
            currentBoss.health -= 10;
            ballPosition = { 10, 10 };
        }
        if (CheckCollisionCircleLine(ballPosition, ballRadius, collsionLinePoint1, collsionLinePoint2)) {
            std::cout << "bounce\n";
            Vector2 bossPos = { boss_x, boss_y };
            Vector2 directionToBoss = Vector2Subtract(bossPos, ballPosition);
            /*ballSpeed.x *= 1;
            ballSpeed.y *= -1;*/
            /*directionToBoss.x /= 100;
            directionToBoss.y /= 100;*/

            
            ballSpeed = Vector2Normalize(directionToBoss);
            ballSpeed = Vector2Scale(ballSpeed, initialBallSpeed);

        }

        if (currentBoss.health <= 0) {
            bool moreBosses = Bosses::AdvanceToNext(game);
            if (!moreBosses)
                world_complete = true;
        }

        if (world_complete)
            return WORLD_COMPLETED;
        else
            return WORLD_IN_PROGRESS;
    }

    void Draw(const GameState& game) {
        DrawCircleLines(400,300, 275, PURPLE);
        // draw background text
        int text_x = 250;
        int text_y = 100;
        DrawText("Evan and Richie World", text_x, text_y, 25, PURPLE);

        
        // draw player square
       // DrawRectangle(player_x, player_y, player_width, player_height, RED);
        

        // get the current boss and draw it
        const BossState& currentBoss = Bosses::ActiveBoss(game);
        int boss_size = 75 * boss_scale;
        Bosses::Draw(currentBoss, boss_x, boss_y, boss_scale);
        Bosses::DrawHealthBar(currentBoss, boss_x - boss_size, boss_y + boss_size, boss_size * 2);

        //draw ball
        DrawCircleV(ballPosition, ballRadius, BLUE);
        
        
        //testing drawing paddle at rotated location (draw here for z order)
        Rectangle player_rect_on_circle = { player_point_on_circle.x, player_point_on_circle.y,
                                            player_width, player_height };
        //DrawRectangleRec(player_rect_on_circle, BLUE); //just draw a regular rectangle at rotated point
        
        //drawing rectangle rotated.. you specifcy an offset relative to top left corner to rotate rectangle arround
        //here are some possible pivots (relative to rectangel and rectangle size)
        Vector2 pivotTopLeft     = { 0, 0 };
        Vector2 pivotCenter      = { player_width / 2, player_height / 2 };
        Vector2 pivotBottomRight = { player_width , player_height };
        
        //draw each of these so we can see
       // DrawRectanglePro(player_rect_on_circle, pivotTopLeft, player_rotation, MAGENTA);
        DrawRectanglePro(player_rect_on_circle, pivotCenter, player_rotation, GREEN);
       // DrawRectanglePro(player_rect_on_circle, pivotBottomRight, player_rotation, PURPLE);

        //draw the collision line so we can see it
        //DrawLineV(collsionLinePoint1, collsionLinePoint2, YELLOW);


        //testing drawing a point at right radius
        DrawCircleV(PLAYER_START_POINT, 5, WHITE);
        DrawCircleV(player_point_on_circle, 5, YELLOW);
    }

    //rotates given point around center by given angle (degrees) and returns rotated point
    Vector2 RotatePointAroundCenter(Vector2 point, float angleDegrees) {
        //rotation function will be around (0,0) (top left corner) by default, so move to center
        point.x -= CENTER_X;
        point.y -= CENTER_Y;

        Vector2 rotatedPoint = Vector2Rotate(point, angleDegrees * DEG2RAD); //function expects radians, convert from degrees

        //add back to go to original coordinate system
        rotatedPoint.x += CENTER_X;
        rotatedPoint.y += CENTER_Y;

        return rotatedPoint;
    }

}