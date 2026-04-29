#include <iostream>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION //only in this cpp file
#include "raygui.h" // in any cpp file that needs it
#include "raymath.h" //in any cpp file that needs it

#include "shared.h"
#include "bosses.h"
#include "overworld.h"
#include "world0.h"
#include "world1.h"
#include "world2.h"
#include "world3.h"
#include "world4.h"
#include "world5.h"
#include "world6.h"



// ----- Game Modes -----
enum GameMode {
    GAME_MODE_OVERWORLD = -1,
    GAME_MODE_WORLD_0 = 0,
    GAME_MODE_WORLD_1,
    GAME_MODE_WORLD_2,
    GAME_MODE_WORLD_3,
    GAME_MODE_WORLD_4,
    GAME_MODE_WORLD_5,
    GAME_MODE_WORLD_6,
    GAME_MODE_GAME_OVER,
    GAME_MODE_WIN_SCREEN
};

void InitCorrectWorld(GameMode gm);
void UpdateCorrectWorld(GameMode &gm, GameState &gs);
void DrawCorrectWorld(GameMode gm, GameState& gs);

//helper functions
void DrawWorldOverlay(GameState& gs);
void WorldTimerUpdate(GameState& gs, GameMode& gm);
void ChangeMusicTrack(Music& currentTrack, const Music& newTrack);


int main() {
    GameMode currentGameMode = GAME_MODE_OVERWORLD;
    GameState gameState; //check defualt values in shared.h
    Bosses::InitAll(gameState);

    ChangeDirectory( GetApplicationDirectory() );//assumption we make for assets
    InitAudioDevice();
    gameState.menuTrack = LoadMusicStream("assets/music/dystopia.mp3");
    gameState.worldTrack = LoadMusicStream("assets/music/active_camo.mp3");
    gameState.currentTrack = gameState.menuTrack;
    PlayMusicStream(gameState.currentTrack);
    PauseMusicStream(gameState.currentTrack);
    gameState.musicPlaying = false;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mini Boss Dungeon Game Collection");
    SetExitKey(KEY_NULL); //disable default ESC behavior
    SetTargetFPS(60);
    
    while ( !WindowShouldClose() ) { //start main loop
        UpdateMusicStream(gameState.currentTrack);
        // UPDATING & INITIALIZING NEXT WORLD IF NEEDED
        UpdateCorrectWorld(currentGameMode, gameState);
        
        //DRAWING
        BeginDrawing();
        //BEGIN DRAWING
            ClearBackground(BG_COLOR);         
            DrawCorrectWorld(currentGameMode, gameState);
                        
            // Global UI Overlay
            DrawText(TextFormat("Score: %d", gameState.score), 10, 10, 20, PINK);
            DrawText(TextFormat("HP: %i", gameState.health), 10, 40, 20, RED);
            DrawText("Toggle Music: M", 10, SCREEN_HEIGHT - 30, 20, ORANGE);
            //Overlay only when inside worlds
            if (currentGameMode >= 0)
                DrawWorldOverlay(gameState); //shows time remaining.. boss health?
        //END DRAWING
        EndDrawing();
    }// end main window loop

    UnloadMusicStream(gameState.menuTrack);
    UnloadMusicStream(gameState.worldTrack);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void DrawWorldOverlay(GameState& gs) {
    DrawText(TextFormat("Time: %4.1f", gs.worldTimeRemaining), SCREEN_WIDTH - 100, 10, 20, WHITE);
}

void InitCorrectWorld(GameMode gm) {
    switch (gm) {
        case GAME_MODE_WORLD_0:
            World0::Init();
            break;
        case GAME_MODE_WORLD_1:
            World1::Init();
            break;
        case GAME_MODE_WORLD_2:
            World2::Init();
            break;
        case GAME_MODE_WORLD_3:
            World3::Init();
            break;
        case GAME_MODE_WORLD_4:
            World4::Init();
            break;
        case GAME_MODE_WORLD_5:
            World5::Init();
            break;
        case GAME_MODE_WORLD_6:
            World6::Init();
            break;
    }

}

void DrawCorrectWorld(GameMode gm, GameState& gs) {
    switch (gm) {
        case GAME_MODE_OVERWORLD: 
            DrawOverworld();
            break;
        case GAME_MODE_WORLD_0:
            World0::Draw(gs);
            break;
        case GAME_MODE_WORLD_1:
            World1::Draw(gs);
            break;
        case GAME_MODE_WORLD_2:
            World2::Draw(gs);
            break;
        case GAME_MODE_WORLD_3:
            World3::Draw(gs);
            break;
        case GAME_MODE_WORLD_4:
            World4::Draw(gs);
            break;
        case GAME_MODE_WORLD_5:
            World5::Draw();
            break;
        case GAME_MODE_WORLD_6:
            World6::Draw(gs);
            break;
        case GAME_MODE_WIN_SCREEN:
            //
            break;
        default:
            DrawOverworld();
    }
}


void UpdateCorrectWorld(GameMode &gm, GameState &gs) {
    //some key press stuff that is handled the same no matter game mode    
    bool pressedESC = IsKeyPressed(KEY_ESCAPE); 
    
    //mute / unmute music
    if (IsKeyPressed(KEY_M)) {
        gs.musicPlaying = !gs.musicPlaying; //flip values on every key press
        if (gs.musicPlaying)
            ResumeMusicStream(gs.currentTrack);
        else
            PauseMusicStream(gs.currentTrack);


    }
    
    if (pressedESC) ChangeMusicTrack(gs.currentTrack, gs.menuTrack); //a little hack to catch case of escape pressed and switch music back

    //game mode specific updates and switching game modes
    switch (gm) {
        case GAME_MODE_OVERWORLD:
            if (pressedESC) exit(0);

            gm = (GameMode) UpdateOverworld(gs); //hack, should clean up
            InitCorrectWorld(gm);
            if (gm >= GAME_MODE_WORLD_0) { //switching from overworld to game world
                gs.worldTimeRemaining = SECONDS_PER_LEVEL; //set up initial timer for each level
                ChangeMusicTrack(gs.currentTrack, gs.worldTrack);
            }
            break;
        case GAME_MODE_WORLD_0:
            if (World0::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD; //this should go to win screen when we have one
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_1:
            if (World1::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_2:
            if (World2::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_3:
            if (World3::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_4:
            if (World4::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_5:
            if (World5::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WORLD_6:
            if (World6::Update(gs) == WORLD_COMPLETED || pressedESC)
                gm = GAME_MODE_OVERWORLD;
            else
                WorldTimerUpdate(gs, gm);
            break;
        case GAME_MODE_WIN_SCREEN:
            //
            break;
    }
}

void WorldTimerUpdate(GameState& gs, GameMode& gm) {
    gs.worldTimeRemaining -= GetFrameTime();
    if (gs.worldTimeRemaining <= 0) {
        gm = GAME_MODE_OVERWORLD;
        ChangeMusicTrack(gs.currentTrack, gs.menuTrack);
    }
}

void ChangeMusicTrack(Music& currentTrack, const Music &newTrack) {
    StopMusicStream(currentTrack);
    currentTrack = newTrack;
    PlayMusicStream(currentTrack);
}
