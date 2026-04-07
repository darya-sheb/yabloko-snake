#include "ai.h"
#include "../lib.h"

static void bfs_init(int visited[FIELD_W][FIELD_H], int prev[FIELD_W][FIELD_H][2]) {
    for (int i = 0; i < FIELD_W; i++)
        for (int j = 0; j < FIELD_H; j++) {
            visited[i][j] = 0;
            prev[i][j][0] = -1;
            prev[i][j][1] = -1;
        }
}

void AIPlayer::getDirection(const Snake& snake, const World& world, int& dirX, int& dirY) {
    const Point head = snake.head();
    int bestDist = 10000;
    Point target = world.getApple(0);
    for (int i = 0; i < world.appleCount(); i++) {
        Point a = world.getApple(i);
        int d = (head.x - a.x)*(head.x - a.x) + (head.y - a.y)*(head.y - a.y);
        if (d < bestDist) {
            bestDist = d;
            target = a;
        }
    }

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    int visited[FIELD_W][FIELD_H];
    int prev[FIELD_W][FIELD_H][2];
    bfs_init(visited, prev);

    int queue[FIELD_W*FIELD_H][2];
    int qhead = 0, qtail = 0;
    queue[qtail][0] = head.x;
    queue[qtail][1] = head.y;
    qtail++;
    visited[head.x][head.y] = 1;

    while (qhead < qtail) {
        int x = queue[qhead][0];
        int y = queue[qhead][1];
        qhead++;
        if (x == target.x && y == target.y) break;
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx < 0) nx = FIELD_W-1;
            if (nx >= FIELD_W) nx = 0;
            if (ny < 0) ny = FIELD_H-1;
            if (ny >= FIELD_H) ny = 0;
            if (!visited[nx][ny]) {
                bool blocked = false;
                if (world.isObstacle(nx, ny)) blocked = true;
                for (int i = 1; i < snake.length(); i++) {
                    if (snake.segment(i).x == nx && snake.segment(i).y == ny) {
                        blocked = true;
                        break;
                    }
                }
                if (!blocked) {
                    visited[nx][ny] = 1;
                    prev[nx][ny][0] = x;
                    prev[nx][ny][1] = y;
                    queue[qtail][0] = nx;
                    queue[qtail][1] = ny;
                    qtail++;
                }
            }
        }
    }

    int cx = target.x, cy = target.y;
    while (prev[cx][cy][0] != head.x || prev[cx][cy][1] != head.y) {
        int nx = prev[cx][cy][0];
        int ny = prev[cx][cy][1];
        if (nx == -1 || ny == -1) return;
        cx = nx; cy = ny;
    }
    int ndx = cx - head.x;
    int ndy = cy - head.y;
    if (ndx == -FIELD_W+1) ndx = 1;
    if (ndx == FIELD_W-1) ndx = -1;
    if (ndy == -FIELD_H+1) ndy = 1;
    if (ndy == FIELD_H-1) ndy = -1;
    if (ndx != 0 || ndy != 0) {
        dirX = ndx;
        dirY = ndy;
    }
}