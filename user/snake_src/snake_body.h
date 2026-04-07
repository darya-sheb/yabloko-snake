#pragma once
#include "snake.h"

class Snake {
public:
    Snake();
    void init();
    bool move(int dirX, int dirY, bool ate);
    bool checkSelfCollision() const;
    void grow();
    const Point& head() const { return body[0]; }
    int length() const { return len; }
    Point segment(int i) const { return body[i]; }
private:
    Point body[MAX_SNAKE_LEN];
    int len;
};