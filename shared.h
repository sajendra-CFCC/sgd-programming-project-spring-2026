#pragma once

#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const Color BG_COLOR = BLACK;

struct GameState {
    int score = 0;
    int health = 100;
    int keysCollected = 0;
    Vector2 position = {0, 0}; //hub world position? consistent position?
    bool isAlive = true; //implement game over? reset?
    
};

enum WorldUpdateResult {
    WORLD_COMPLETED,
    WORLD_IN_PROGRESS
};