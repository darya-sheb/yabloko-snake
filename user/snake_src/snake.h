#pragma once

#include <stdint.h>


constexpr int CELL_SIZE = 10;
constexpr int FIELD_W = 32;
constexpr int FIELD_H = 20;
constexpr int SCREEN_W = FIELD_W * CELL_SIZE;   // 320
constexpr int SCREEN_H = FIELD_H * CELL_SIZE;   // 200

constexpr int MAX_SNAKE_LEN = FIELD_W * FIELD_H;
constexpr int DEFAULT_APPLES = 5;
constexpr int TICK_NORMAL_MS = 150;
constexpr int TICK_FAST_MS = 50;
constexpr int OBSTACLE_COUNT = 5;


enum ScanCode : uint8_t {
    SCAN_W = 0x11,
    SCAN_A = 0x1E,
    SCAN_S = 0x1F,
    SCAN_D = 0x20,
    SCAN_SPACE = 0x39,
    SCAN_R = 0x13,
    SCAN_F1 = 0x3B,
};

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};