#pragma once
#include "snake.h"
#include "snake_body.h"
#include "world.h"

void render_init(uint8_t* vram);
void clear_screen();
void draw_cell(int cx, int cy, uint8_t main_color, uint8_t shadow_color);
void draw_text(int x, int y, const char* str, uint8_t color);
void draw_game(const Snake& snake, const World& world, int score);