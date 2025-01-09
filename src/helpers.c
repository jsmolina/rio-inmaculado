#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>
#include <math.h>



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
