#include "tiles.h"
#include "allegro/datafile.h"
#include "statics.h"
#include "helpers.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "dat_manager.h"

PALETTE palette;


struct coords get_tile_coords(int tile_number) {
    struct coords result;
    result.x = 8 * ((tile_number - 1) % 32);
    result.y = 8 * ((tile_number - 1) >> 5);

    return result;
}
inline void load_tiles(DATAFILE *dat_file) {
    tiles = dat_file[TILES_BMP].dat;
    dat_palette(PALETE_TILES_BMP, palette);
   // generate_optimized_palette(tiles, palette, 0); 

    if(!tiles) {
        die("cannot load tiles");
    }
}

inline void destroy_tiles() {
    // destroy_bitmap(tiles);
}

BITMAP * load_background(int id) {
    char *in_file = dat_file[id].dat;
    if (in_file == NULL) {
        die("cannot load %s", id);
    }
    BITMAP * background = create_bitmap(SCREEN_W, SCREEN_H);
    rectfill(background, 0, 0, SCREEN_W, SCREEN_H, makecol(40, 40, 40));
    // FILE *in_file  = fopen(filename, "r");
    char current;

    // if (!in_file) {
    //     die("ops, file <%s> can't be read", filename);
    //}

    // skip xml data
    int start_csv = 0;
    int i = 0;

    do {
        current = in_file[i++];
        if (current == '>') {
            start_csv += 1;
        }
    } while (start_csv < 5 && current != '\0');

    // temporal data for csv
    char current_tile[5] = "     ";
    int charpos = 0;
    char * output;
    int iterations = 0;
    // current screen position
    struct coords screen_coords;
    screen_coords.x = screen_coords.y = 0;

    do {
        current = in_file[i++];

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
            blit(tiles, background, coordinates.x, coordinates.y,
                 screen_coords.x, screen_coords.y, TILES_SIZE, TILES_SIZE);

            screen_coords.x += 8;
            if (screen_coords.x >= 319) {
                screen_coords.y += 8;
                screen_coords.x = 0;
            }

        } else if (current != '\0') { // a number
            current_tile[charpos++] = current;
        }

        iterations += 1;
        // if (iterations == 200) start_csv = -1; // Uncomment if needed
    } while (current != '\0' && start_csv != -1);


    return background;
}
