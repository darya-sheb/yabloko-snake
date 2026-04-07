#pragma once

#include <stdint.h>

void init_keyboard();

extern unsigned kbd_buf_size;
extern char *kbd_buf;

#define KBD_EVENT_BUFFER_SIZE 64

struct kbd_event {
    uint8_t scancode;
    uint8_t pressed;
};

void kbd_get_event(struct kbd_event* ev);
int kbd_get_key_state(uint8_t scancode);