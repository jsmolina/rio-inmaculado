#include <allegro.h>

#ifndef DAT_MANAGER
#define DAT_MANAGER

extern DATAFILE *dat_file;
// unpacks dat file
DATAFILE * extract_data();
// cleanups data
void cleanup_data();
// frame of a sprite sheet packed in datos.dat (a sub-bitmap, sheets are cut in code)
BITMAP *dat_frame(int id, int x, int w, int h);
// copy of a packed bitmap, for callers that destroy_bitmap() it later
BITMAP *dat_copy(int id);
// copies a packed palette (PAL object) into pal
void dat_palette(int id, PALETTE pal);

#endif
