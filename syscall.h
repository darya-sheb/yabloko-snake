#pragma once

enum {
	T_SYSCALL = 0x84,
	SYS_exit = 0,
	SYS_greet = 1,
	SYS_putc = 2,
	SYS_puts = 3,
	SYS_get_event = 4,
    SYS_get_key_state = 5,
    SYS_get_ticks = 6,
    SYS_sleep = 7,
    SYS_beep = 8,
    SYS_setgraphics = 9,
    SYS_setconsole = 10,
};

int syscall(int call, int arg);
