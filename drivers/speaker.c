#include "speaker.h"
#include "port.h"
#include "pit.h"

void beep(int duration_ms) {
    uint32_t freq = 440;
    uint32_t divisor = 1193180 / freq;
    port_byte_out(0x43, 0xb6);
    port_byte_out(0x42, divisor & 0xFF);
    port_byte_out(0x42, (divisor >> 8) & 0xFF);
    uint8_t tmp = port_byte_in(0x61);
    port_byte_out(0x61, tmp | 0x03);
    msleep(duration_ms);
    tmp = port_byte_in(0x61);
    port_byte_out(0x61, tmp & ~0x03);
}