#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILENAME_LEN 16

// Estructura para almacenar la cabecera de cada archivo (nombre y tamaño)
typedef struct {
    char filename[MAX_FILENAME_LEN];
    long long filesize;
} FileHeader;

void decompress_files() {
    FILE *input = fopen("DATA.DAT", "rb");
    if (!input) {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
    }
    chdir("data");

    while (1) {
        FileHeader header;
        // Leer la cabecera
        size_t read_size = fread(&header, sizeof(FileHeader), 1, input);
        if (read_size != 1) {
            if (feof(input))
                break;
            perror("Error al leer la cabecera");
            exit(EXIT_FAILURE);
        }

        // Abrir el archivo de salida con el nombre original
        FILE *output = fopen(header.filename, "wb");
        if (!output) {
            perror("Error al abrir el archivo de salida");
            exit(EXIT_FAILURE);
        }

        // Leer y escribir los datos del archivo
        size_t block_size = 4096; // 4 KB
        char *buffer = (char *)malloc(block_size);
        size_t bytes_read;
        size_t bytes_remaining = header.filesize;

        while (bytes_remaining > 0) {
            if (bytes_remaining > block_size) {
                bytes_read = fread(buffer, 1, block_size, input);
            } else {
                bytes_read = fread(buffer, 1, bytes_remaining, input);
            }
            printf("Read %zu, ", bytes_read);
            fwrite(buffer, 1, bytes_read, output);
            bytes_remaining = bytes_remaining - bytes_read;
            printf("Remaining %zu, ", bytes_remaining);
        }
        free(buffer);

        fclose(output);

        // fseek(input, header.filesize, SEEK_CUR);
        printf("Archivo '%s' descomprimido.\n", header.filename);
        printf("Seek %lld\n", header.filesize);
    }

    fclose(input);
}

int main() {
    // Desempaquetar archivos desde 'data.dat'
    decompress_files();

    return 0;
}