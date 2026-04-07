#include "snake_body.h"

Snake::Snake() { init(); }

void Snake::init() {
    len = 3;
    body[0] = Point(FIELD_W/2, FIELD_H/2);
    body[1] = Point(FIELD_W/2 - 1, FIELD_H/2);
    body[2] = Point(FIELD_W/2 - 2, FIELD_H/2);
}

bool Snake::move(int dirX, int dirY, bool ate) {
    
    Point newHead(body[0].x + dirX, body[0].y + dirY);

    if (newHead.x < 0) newHead.x = FIELD_W - 1;
    if (newHead.x >= FIELD_W) newHead.x = 0;
    if (newHead.y < 0) newHead.y = FIELD_H - 1;
    if (newHead.y >= FIELD_H) newHead.y = 0;

    if (ate) {
        for (int i = len; i > 0; i--) {
            body[i] = body[i-1];
        }
        body[0] = newHead;
        len++;
        if (len > MAX_SNAKE_LEN) len = MAX_SNAKE_LEN;
    } else {
        for (int i = len-1; i > 0; i--) {
            body[i] = body[i-1];
        }
        body[0] = newHead;
    }
    return true;
}

bool Snake::checkSelfCollision() const {
    for (int i = 1; i < len; i++) {
        if (body[0].x == body[i].x && body[0].y == body[i].y)
            return true;
    }
    return false;
}

void Snake::grow() {
    if (len < MAX_SNAKE_LEN) {
        body[len] = body[len-1];
        len++;
    }
}