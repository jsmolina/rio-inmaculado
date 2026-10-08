#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "dat_manager.h"


DATAFILE *dat_file;

#define MAX_FILENAME_LEN 16

typedef struct {
    char filename[MAX_FILENAME_LEN];
    long long filesize;
} FileHeader;

static void rotar_paleta() {
    PALETTE pal;
    get_palette(pal);

    for (int i = 200; i < 240; i++) {
        int nuevo_indice = (i + 1) % 256;
        RGB color_temp = pal[i];
        pal[i] = pal[nuevo_indice];
        pal[nuevo_indice] = color_temp;
    }

    set_palette(pal);
}
END_OF_FUNCTION(rotar_paleta)


DATAFILE * extract_data() {

    //install_int(rotar_paleta, 100);
    install_int_ex(rotar_paleta, BPS_TO_TIMER(40));
    dat_file = load_datafile("datos.dat");
    
    /*FILE *input = fopen("DATA.DAT", "rb");
    if (!input) {
        allegro_message("Error al abrir el archivo de entrada");
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
            allegro_message("Error al abrir el archivo de salida %s", header.filename);
            exit(EXIT_FAILURE);
        }

        // Leer y escribir los datos del archivo
        size_t block_size = 4096; // 4 KB
        char *buffer = (char *)malloc(block_size);
        size_t bytes_read;
        long long bytes_remaining = header.filesize;
        int max_iter = 0;

        while (bytes_remaining > 0) {
            if (bytes_remaining > block_size) {
                bytes_read = fread(buffer, 1, block_size, input);
            } else {
                bytes_read = fread(buffer, 1, bytes_remaining, input);
            }
            fwrite(buffer, 1, bytes_read, output);
            bytes_remaining = bytes_remaining - bytes_read;
        }
        free(buffer);

        fclose(output);        
    }*/
    remove_int(rotar_paleta);
    return dat_file;
    //fclose(input);
}

BITMAP *dat_frame(int id, int x, int w, int h) {
    return create_sub_bitmap(dat_file[id].dat, x, 0, w, h);
}

BITMAP *dat_copy(int id) {
    BITMAP *src = dat_file[id].dat;
    BITMAP *copy = create_bitmap(src->w, src->h);
    blit(src, copy, 0, 0, 0, 0, src->w, src->h);
    return copy;
}

void dat_palette(int id, PALETTE pal) {
    memcpy(pal, dat_file[id].dat, sizeof(PALETTE));
}

void cleanup_data() {
    chdir("..");
}