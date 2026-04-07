#include "keyboard.h"
#include "cpu/isr.h"
#include "cpu/memlayout.h"
#include "console.h"
#include "port.h"
#include "kernel/mem.h"
#include <stdbool.h>


static const char sc_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', '\n', '?', 'a', 's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' ',
};


static const char sc_ascii_shift[] = {
    '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
    'U', 'I', 'O', 'P', '{', '}', '\n', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' ',
};

enum { kbd_buf_capacity = PGSIZE };

static struct kbd_event event_buffer[KBD_EVENT_BUFFER_SIZE];
static int event_buffer_head = 0;
static int event_buffer_tail = 0;
static bool key_state[256];
static bool shift_pressed = false;   

char* kbd_buf;
unsigned kbd_buf_size;

void kbd_get_event(struct kbd_event* ev) {
    if (event_buffer_head == event_buffer_tail) {
        ev->scancode = 0;
        ev->pressed = 0;
        return;
    }
    *ev = event_buffer[event_buffer_tail];
    event_buffer_tail = (event_buffer_tail + 1) % KBD_EVENT_BUFFER_SIZE;
}

int kbd_get_key_state(uint8_t scancode) {
    return key_state[scancode] ? 1 : 0;
}


static void process_scancode(uint8_t scancode, uint8_t pressed) {
    
    key_state[scancode] = pressed;

    int next_head = (event_buffer_head + 1) % KBD_EVENT_BUFFER_SIZE;
    if (next_head != event_buffer_tail) {
        event_buffer[event_buffer_head].scancode = scancode;
        event_buffer[event_buffer_head].pressed = pressed;
        event_buffer_head = next_head;
    }

    
    if (scancode == 0x2A || scancode == 0x36) { 
        shift_pressed = pressed;
    }

    
    if (pressed) {
        char c = 0;
        if (shift_pressed && scancode < sizeof(sc_ascii_shift)) {
            c = sc_ascii_shift[scancode];
        } else if (!shift_pressed && scancode < sizeof(sc_ascii)) {
            c = sc_ascii[scancode];
        }

        if (c == '\n') {
            if (kbd_buf_size < kbd_buf_capacity) {
                kbd_buf[kbd_buf_size++] = c;
            }
            printk("\n");
        } else if (c == '\b') {   
            if (kbd_buf_size > 0) {
                kbd_buf_size--;
                printk("\b \b");
            }
        } else if (c != '?') {
            if (kbd_buf_size < kbd_buf_capacity) {
                kbd_buf[kbd_buf_size++] = c;
            }
            char str[2] = {c, 0};
            printk(str);
        }
    }
}

static void interrupt_handler(registers_t *r) {
    uint8_t scancode = port_byte_in(0x60);
    uint8_t pressed = 1;
    if (scancode & 0x80) {
        pressed = 0;
        scancode &= 0x7F;
    }
    process_scancode(scancode, pressed);
}

void init_keyboard() {
    kbd_buf = kalloc();
    for (int i = 0; i < 256; i++) key_state[i] = false;
    shift_pressed = false;
    event_buffer_head = event_buffer_tail = 0;
    register_interrupt_handler(IRQ1, interrupt_handler);
}