#include "game.h"
#include "render.h"
#include "../lib.h"

Game::Game() : gameOver(false), autoPlay(false), restartRequest(false), score(0),
               dirX(1), dirY(0), nextDirX(1), nextDirY(0) {
    vram = nullptr;
}

void Game::init() {
    snake.init();
    world.init(snake);
    score = 0;
    gameOver = false;
    dirX = 1; dirY = 0;
    nextDirX = 1; nextDirY = 0;
}

void Game::showMenu() {
    clear_screen();
    draw_text(SCREEN_W/2 - 4*8, 40, "SNAKE", 0x0F);
    draw_text(SCREEN_W/2 - 12*8, 70, "Press any key", 0x0A);
    draw_text(SCREEN_W/2 - 15*8, 100, "R - restart, F1 - auto", 0x07);
    draw_text(SCREEN_W/2 - 12*8, 130, "WASD + Space", 0x07);
    struct kbd_event ev;
    while (1) {
        if (get_event(&ev) == 0 && ev.pressed) {
            if (ev.scancode == SCAN_R) restartRequest = true;
            if (ev.scancode == SCAN_F1) autoPlay = true;
            break;
        }
        sleep_ms(10);
    }
}

void Game::showGameOver() {
    clear_screen();
    draw_text(SCREEN_W/2 - 6*8, 60, "GAME OVER", 0x0C);
    char buf[32];
    draw_text(SCREEN_W/2 - 7*8, 90, "Score: ", 0x0F);
    int n = score;
    char* p = buf + 31;
    *p = 0;
    do { *--p = '0' + (n%10); n/=10; } while(n);
    draw_text(SCREEN_W/2 - 7*8 + 6*8, 90, p, 0x0F);
    draw_text(SCREEN_W/2 - 12*8, 120, "Press R to restart", 0x0A);
    while (1) {
        struct kbd_event ev;
        if (get_event(&ev) == 0 && ev.pressed && ev.scancode == SCAN_R) {
            restartRequest = true;
            break;
        }
        sleep_ms(10);
    }
}

void Game::handleInput() {
    struct kbd_event ev;
    while (get_event(&ev) == 0) {
        if (ev.pressed) {
            switch (ev.scancode) {
                case SCAN_W: if (dirY != 1)  { nextDirX = 0; nextDirY = -1; } break;
                case SCAN_S: if (dirY != -1) { nextDirX = 0; nextDirY = 1; } break;
                case SCAN_A: if (dirX != 1)  { nextDirX = -1; nextDirY = 0; } break;
                case SCAN_D: if (dirX != -1) { nextDirX = 1; nextDirY = 0; } break;
                case SCAN_R: gameOver = true; restartRequest = true; break;
                case SCAN_F1: autoPlay = !autoPlay; break;
            }
        }
    }
}

void Game::update() {
    if (!(nextDirX == -dirX && nextDirY == -dirY)) {
        dirX = nextDirX;
        dirY = nextDirY;
    }


    int newHeadX = snake.head().x + dirX;
    int newHeadY = snake.head().y + dirY;

    if (newHeadX < 0) newHeadX = FIELD_W - 1;
    if (newHeadX >= FIELD_W) newHeadX = 0;
    if (newHeadY < 0) newHeadY = FIELD_H - 1;
    if (newHeadY >= FIELD_H) newHeadY = 0;

    int appleIndex;
    bool ate = world.isApple(newHeadX, newHeadY, appleIndex);

    snake.move(dirX, dirY, ate);


    if (ate) {
        score++;
        beep_ms(80);
        world.eatApple(newHeadX, newHeadY);

        if (world.appleCount() == 0) {
            gameOver = true;
            return;
        }
        world.generateApple(snake);
    }

    if (world.isObstacle(snake.head().x, snake.head().y))
        gameOver = true;

    if (snake.checkSelfCollision())
        gameOver = true;
}

void Game::draw() {
    draw_game(snake, world, score);
}

void Game::run() {

    vram = (uint8_t*)set_graphics_mode(0x500000);
    if (vram == nullptr) {
        set_console_mode();
        return;
    }
    render_init(vram);

    while (1) {
        init();
        showMenu();
        if (restartRequest) {
            restartRequest = false;
            continue;
        }

        uint32_t lastTick = get_ticks();
        int speedMs = TICK_NORMAL_MS;
        while (!gameOver) {
            handleInput();

            bool spacePressed = get_key_state(SCAN_SPACE);
            speedMs = spacePressed ? TICK_FAST_MS : TICK_NORMAL_MS;

            if (autoPlay) {
                int adx = dirX, ady = dirY;
                ai.getDirection(snake, world, adx, ady);
                if (!(adx == -dirX && ady == -dirY)) {
                    nextDirX = adx;
                    nextDirY = ady;
                }
            }

            uint32_t now = get_ticks();
            uint32_t elapsed = (now - lastTick) * 10;   // тики * 10 мс
            if (elapsed < (uint32_t)speedMs)
                sleep_ms(speedMs - elapsed);
            lastTick = get_ticks();

            update();
            draw();
        }

        showGameOver();
        if (!restartRequest) break;
        restartRequest = false;
    }

    set_console_mode();
}