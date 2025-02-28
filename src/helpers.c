#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>
#include <math.h>
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


char is_cpu_slow() {
    clock_t start, end;
    double cpu_time_used;
    int i;
    volatile int dummy = 0;

    // Medir tiempo de una operación simple
    start = clock();
    for (i = 0; i < 100000; i++) {
        dummy += i;
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Interpretar resultados
    if (cpu_time_used < 0.10) {
        return FALSE;
    } else {
        return TRUE;
    }
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
