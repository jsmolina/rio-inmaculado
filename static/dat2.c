#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LEN 16

// Estructura para almacenar la cabecera de cada archivo (nombre y tamaño)
typedef struct {
    char filename[MAX_FILENAME_LEN];
    long long filesize;
} FileHeader;

void concatenate_files(int argc, char *argv[]) {
    FILE *output = fopen("DATA.DAT", "wb");
    if (!output) {
        perror("Error al abrir el archivo de salida");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        printf("\nCompressing %s ... \n", argv[i]);
        FILE *input = fopen(argv[i], "rb");
        if (!input) {
            perror("Error al abrir el archivo de entrada");
            exit(EXIT_FAILURE);
        }

        // Obtener el tamaño del archivo de entrada
        fseek(input, 0, SEEK_END);
        long filesize = ftell(input);
        fseek(input, 0, SEEK_SET);

        // Escribir la cabecera con el nombre y el tamaño del archivo
        FileHeader header;
        strncpy(header.filename, argv[i], MAX_FILENAME_LEN);
        header.filename[MAX_FILENAME_LEN - 1] = '\0';

        header.filesize = filesize;
        printf("Filesize: %lld\n", header.filesize);
        fwrite(&header, sizeof(FileHeader), 1, output);

        // Copiar el contenido del archivo al archivo de salida
        char *buffer = (char *)malloc(filesize);
        if (!buffer) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }
        fread(buffer, 1, filesize, input);
        fwrite(buffer, 1, filesize, output);
        free(buffer);

        fclose(input);
    }

    fclose(output);
    printf("Archivos concatenados en 'DATA.DAT'.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo1> <archivo2> ... <archivoN>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    concatenate_files(argc, argv);

    return 0;
}