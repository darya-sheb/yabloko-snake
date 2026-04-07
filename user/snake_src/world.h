#pragma once
#include "snake.h"
#include "snake_body.h"

class World {
public:
    World();
    void init(const Snake& snake);
    bool isObstacle(int x, int y) const;
    bool isApple(int x, int y, int& index) const;
    int eatApple(int x, int y);
    void generateApple(const Snake& snake);
    int appleCount() const { return appleCnt; }
    Point getApple(int i) const { return apples[i]; }
    Point getObstacle(int i) const { return obstacles[i]; }
private:
    Point apples[DEFAULT_APPLES];
    int appleCnt;
    Point obstacles[OBSTACLE_COUNT];
};