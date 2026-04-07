#pragma once
#include "../syscall.h"
#include "../drivers/keyboard.h"
#include <stdint.h>

static inline int syscall0(int num) {
    int ret;
    asm volatile("int $0x84" : "=a"(ret) : "a"(num) : "memory");
    return ret;
}

static inline int syscall1(int num, int arg1) {
    int ret;
    asm volatile("int $0x84" : "=a"(ret) : "a"(num), "b"(arg1) : "memory");
    return ret;
}

static inline int syscall2(int num, int arg1, int arg2) {
    int ret;
    asm volatile("int $0x84" : "=a"(ret) : "a"(num), "b"(arg1), "c"(arg2) : "memory");
    return ret;
}

static inline int get_event(struct kbd_event *ev) {
    return syscall1(SYS_get_event, (int)ev);
}

static inline int get_key_state(int scancode) {
    return syscall1(SYS_get_key_state, scancode);
}

static inline uint32_t get_ticks(void) {
    return syscall0(SYS_get_ticks);
}

static inline void sleep_ms(int ms) {
    syscall1(SYS_sleep, ms);
}

static inline void beep_ms(int ms) {
    syscall1(SYS_beep, ms);
}

static inline void* set_graphics_mode(uint32_t user_vaddr) {
    return (void*)syscall1(SYS_setgraphics, user_vaddr);
}

static inline void set_console_mode(void) {
    syscall0(SYS_setconsole);
}