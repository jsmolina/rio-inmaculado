#ifndef TILES_H
#define TILES_H

#include <allegro.h>

#define TILES_SIZE 8


struct coords {
    int x;
    int y;
};

/**
Tiles image
*/
extern BITMAP *tiles;

/**
Preloads all tiles as a bitmap to blit them to screen
*/
extern inline void load_tiles();
extern inline void destroy_tiles();
extern BITMAP * load_background(char * filename);

/**
Returns x and y positions from a tile number
*/
extern struct coords get_tile_coords(int tile_number);


#endif