#ifndef GAME_H
#define GAME_H

#include <allegro/gfx.h>
#include <allegro.h>
#include <allegro/keyboard.h>
#define TRANS makecol(255, 0, 255)

extern int exit_game;                   /* flag we'll set to end the game */
extern int x;
extern int y;
extern BITMAP *player[11];
extern BITMAP *bg;
extern int curr_sprite;

extern void input();
extern void process();
extern void output();

#endif
