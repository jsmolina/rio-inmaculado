#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>
#include <math.h>
#include <stdio.h>



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

void cargar_niveles(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return;
    }

    /*while (fscanf(archivo, "%d,%d,%d,%d,%d,%d", 
                  &niveles[total_niveles].nivel,
                  &niveles[total_niveles].puertas[0],
                  &niveles[total_niveles].puertas[1],
                  &niveles[total_niveles].puertas[2],
                  &niveles[total_niveles].puertas[3],
                  &niveles[total_niveles].num_enemigos) == 6) {
        total_niveles++;
        if (total_niveles >= MAX_NIVELES) break;
    }*/

    fclose(archivo);
}
