#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct dirent *direntry;
struct stat s;
unsigned char nfiles = 0;
DIR *folder;
struct stat st = {0};


int check_ext(unsigned char *name) {
    int type = 0;
    int len = strlen((const char *)name);
    unsigned long *ext = (unsigned long *)&name[len - 4];
    switch (ext[0]) {
    // file extensions
    case 0x504D422E:
    case 0x706D622E:
        printf("A BMP\n");
        type = 1;
        break;
    case 0x584D542E:
    case 0x786D742E:
        printf("A TMX\n");
        type = 2;
        break;
    case 0x4D47562E:
    case 0x6D67762E:
        printf("A VGM\n");
        type = 3;
        break;
    }
    return type;
}

unsigned int extract_data() {
    unsigned short filesize = 0x0000;
    unsigned long dat_offset = 32 + 2048;
    unsigned long file_pos = 0;
    unsigned char i, arg;
    unsigned char ssize = 0;
    unsigned char extract = 0;
    unsigned char *extract_file;
    unsigned int number = 0;
    FILE *dat;
    FILE *file;
    unsigned char *filename = (unsigned char *)calloc(1, 65535);


    // EXTRACT FILES
    dat = fopen("DATA.DAT", "rb");
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
    }
    chdir("data");

    fseek(dat, 22, SEEK_SET);
    fread(&number, 1, 1, dat);
    for (i = 1; i < number + 1; i++) {
        int type = 0;
        fseek(dat, 32 * i, SEEK_SET);
        memset(filename, 0, 16);
        // Read file name
        fread(filename, 1, 16, dat);
        printf("Extracting: %s\n", filename);
        // Read data offset
        fread(&dat_offset, 4, 1, dat);
        // Check file type
        type = check_ext(filename);
        // read file size
        fread(&filesize, 2, 1, dat);
        // Go to data offset
        fseek(dat, dat_offset, SEEK_SET);
        // Open file
        file = fopen((const char *)filename, "wb");
        // write file
        fread(filename, filesize, 1, dat);
        fwrite(filename, filesize, 1, file);
        fclose(file);
    }
    fclose(dat);

    free(filename);
    filename = NULL;
    return 0;
}

void cleanup_data() {
    chdir("..");
}