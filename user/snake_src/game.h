#pragma once
#include "snake_body.h"
#include "world.h"
#include "ai.h"

class Game {
public:
    Game();
    void run();
private:
    void init();
    void handleInput();
    void update();
    void draw();
    void showMenu();
    void showGameOver();

    Snake snake;
    World world;
    AIPlayer ai;
    bool gameOver;
    bool autoPlay;
    bool restartRequest;
    int score;
    uint8_t* vram;
    int dirX, dirY, nextDirX, nextDirY;
};