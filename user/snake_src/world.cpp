#include "world.h"
#include "../lib.h"

World::World() {
    obstacles[0] = Point(5,5); obstacles[1] = Point(5,6); obstacles[2] = Point(5,7);
    obstacles[3] = Point(25,15); obstacles[4] = Point(26,15);
}

void World::init(const Snake& snake) {
    appleCnt = DEFAULT_APPLES;
    for (int i = 0; i < DEFAULT_APPLES; i++)
        apples[i] = Point(i*3 % FIELD_W, (i*7) % FIELD_H);
    for (int i = 0; i < DEFAULT_APPLES; i++) {
        int free = 0;
        for (int y = 0; y < FIELD_H; y++)
            for (int x = 0; x < FIELD_W; x++)
                if (!isObstacle(x,y) && !(snake.length()>0 && snake.segment(0).x==x && snake.segment(0).y==y))
                    free++;
        if (free == 0) break;
        int r = (get_ticks() + i*131) % free;
        int cnt = 0;
        for (int y = 0; y < FIELD_H; y++) {
            for (int x = 0; x < FIELD_W; x++) {
                if (!isObstacle(x,y) && !(snake.length()>0 && snake.segment(0).x==x && snake.segment(0).y==y)) {
                    if (cnt == r) {
                        apples[i] = Point(x,y);
                        goto next;
                    }
                    cnt++;
                }
            }
        }
        next:;
    }
}

bool World::isObstacle(int x, int y) const {
    for (int i = 0; i < OBSTACLE_COUNT; i++)
        if (obstacles[i].x == x && obstacles[i].y == y)
            return true;
    return false;
}

bool World::isApple(int x, int y, int& index) const {
    for (int i = 0; i < appleCnt; i++)
        if (apples[i].x == x && apples[i].y == y) {
            index = i;
            return true;
        }
    return false;
}

int World::eatApple(int x, int y) {
    int idx;
    if (isApple(x, y, idx)) {
        for (int i = idx; i < appleCnt-1; i++)
            apples[i] = apples[i+1];
        appleCnt--;
        return 1;
    }
    return 0;
}

void World::generateApple(const Snake& snake) {
    if (appleCnt >= DEFAULT_APPLES) return;
    int freeCells = 0;
    for (int y = 0; y < FIELD_H; y++)
        for (int x = 0; x < FIELD_W; x++)
            if (!isObstacle(x,y) && !snake.checkSelfCollision())
                freeCells++;
    if (freeCells == 0) return;
    int r = get_ticks() % freeCells;
    int cnt = 0;
    for (int y = 0; y < FIELD_H; y++) {
        for (int x = 0; x < FIELD_W; x++) {
            if (!isObstacle(x,y)) {
                bool inSnake = false;
                for (int i = 0; i < snake.length(); i++) {
                    if (snake.segment(i).x == x && snake.segment(i).y == y) {
                        inSnake = true; break;
                    }
                }
                if (!inSnake) {
                    if (cnt == r) {
                        apples[appleCnt++] = Point(x,y);
                        return;
                    }
                    cnt++;
                }
            }
        }
    }
}