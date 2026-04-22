#pragma once

#include "raylib.h"

const float SECONDS_PER_LEVEL = 30;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const Color BG_COLOR = BLACK;

enum BossType {
    BOSS_CREATURE_A = 0,
    BOSS_CREATURE_B = 1,
    BOSS_CREATURE_C = 2,
    BOSS_COUNT = 3
};

struct BossState {
    BossType type;
    int health;
    int maxHealth;
    bool alive;
};

struct GameState {
    int score = 0;
    int health = 100;
    
    //for future use with collectibles?
    int keysCollected = 0;
    
    //timer logic
    float worldTimeRemaining = SECONDS_PER_LEVEL;
    //float totalTime = 0;

    BossState bosses[BOSS_COUNT];   // all bosses, persisted across levels
    int       activeBoss = 0;    // index into bosses[]

    //don't think we are using these
    Vector2 position = {0, 0}; //hub world position? consistent position?
    bool isAlive = true; //implement game over? reset?
};

enum WorldUpdateResult {
    WORLD_COMPLETED, //return this for win situation, defeat all the bosses
    WORLD_IN_PROGRESS,
    //WORLD_TIMEOUT //new state for when time runs out // engine will handle
};