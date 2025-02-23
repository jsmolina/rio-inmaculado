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
extern PALETTE palette;

/**
Preloads all tiles as a bitmap to blit them to screen
*/
inline void load_tiles();
inline void destroy_tiles();
BITMAP * load_background(char * filename);

/**
Returns x and y positions from a tile number
*/
struct coords get_tile_coords(int tile_number);


#endif