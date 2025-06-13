#include <allegro.h>

#ifndef DAT_MANAGER
#define DAT_MANAGER

extern DATAFILE *dat_file;
// unpacks dat file
DATAFILE * extract_data();
// cleanups data
void cleanup_data();

#endif
