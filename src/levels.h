#ifndef LEVELS_H
#define LEVELS_H
#include <allegro.h>


extern unsigned char beep_count;
extern void level_processing();
extern BITMAP * load_level_background(unsigned char lvl);
extern unsigned char move_to_level_if_needed();
extern inline unsigned char is_on_door(int door_x);
extern void move_with_level_limits();
#endif
