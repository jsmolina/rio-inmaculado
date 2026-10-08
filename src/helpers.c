#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>
#include <stdarg.h>
#include <stdio.h>

#include <pc.h>
#include <dos.h>




int random_range(unsigned int low, unsigned int high) {
    return low + (rand() % (high - low));
}

int point_distance(unsigned int x, unsigned int targetX) {
    return abs((int)(x - targetX));
}


void die(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    chdir("..");
    exit(1);
}

void beep(int frequency, int duration) {
    int div = 1193180 / frequency;
    
    outportb(0x43, 0xb6);
    outportb(0x42, div & 0xff);
    outportb(0x42, div >> 8);
    
    outportb(0x61, inportb(0x61) | 3);
    
    delay(duration);    
    outportb(0x61, inportb(0x61) & 0xfc);
}
