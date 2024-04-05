#include "tiles.h"
#include "allegro/gfx.h"
#include "allegro/system.h"
#include <stdio.h>
#include <stdlib.h>

struct coords get_tile_coords(int tile_number) {
    struct coords result;
    result.x = 8 * ((tile_number - 1) % 32);
    result.y = 8 * ((tile_number - 1) >> 5);

    return result;
}

inline void load_tiles() {
    tiles = load_pcx("tiles.pcx", NULL);
}

inline void destroy_tiles() {
    destroy_bitmap(tiles);
}

BITMAP * load_background(char * filename) {
    BITMAP * background = create_bitmap(SCREEN_W, SCREEN_H);
    FILE *in_file  = fopen(filename, "r");
    char current;

    if (!in_file) { 
        allegro_message("ops, file <%s> can't be read", filename);
        exit(1); 
    }
    
    short start_csv = 0;
    // skip xml data
    do {
        current = fgetc(in_file);
        if (current == '>') {
            start_csv += 1;
        }
    } while (start_csv < 5);

    // temporal data for csv
    char current_tile[5] = "     ";
    int charpos = 0;
    char * output;
    int iterations = 0;
    // current screen position
    struct coords screen_coords;
    screen_coords.x = screen_coords.y = 0;

    do {
        current = fgetc(in_file);      
        if (current == ',' || current == '<') {
            if (current == '<') {
                // signal finish reading
                start_csv = -1;
            }
            current_tile[charpos] = '\0';
            charpos = 0;
            int tile_number = strtol(current_tile, &output, 10);
            struct coords coordinates = get_tile_coords(tile_number);
            // copies from tiles to background
            blit(
                tiles, background, 
                coordinates.x, coordinates.y, 
                screen_coords.x, screen_coords.y, 
                TILES_SIZE, TILES_SIZE
            );
            
            screen_coords.x += 8;
            if (screen_coords.x >= 319) {
                screen_coords.y += 8;
                screen_coords.x = 0;
            }
        } else { // a number
            current_tile[charpos] = current;
            charpos += 1;
        }
        iterations += 1;
        if (iterations == 200) {
            //start_csv = -1;
        }
    } while (current != EOF && start_csv != -1);

    fclose(in_file);

    return background;
}
