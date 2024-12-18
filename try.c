#include <stdio.h>
#include <time.h>

#define ITERATIONS 100000

int main() {
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

    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    // Interpretar resultados
    if (cpu_time_used < 0.10) {
        printf("Probablemente es un 486 o superior\n");
    } else {
        printf("Probablemente es un 386 o inferior\n");
    }

    return 0;
}

