#pragma once
#include "snake.h"
#include "snake_body.h"
#include "world.h"

class AIPlayer {
public:
    void getDirection(const Snake& snake, const World& world, int& dirX, int& dirY);
};