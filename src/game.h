#ifndef GAME_H
#define GAME_H

#include <allegro/gfx.h>
#include <allegro.h>
#include <allegro/keyboard.h>

#define TRANS makecol(255, 0, 255)
#define STOPPOS 100
#define STOP_LEFT 101
#define STOP_RIGHT 102
#define MOVING_LEFT 1
#define MOVING_RIGHT 2
#define CROUCH 50
#define JUMPING 52
#define JUMPING_LEFT 3
#define JUMPING_RIGHT 4
#define PUNCH_LEFT 5
#define PUNCH_RIGHT 6

#define ANIM_WALK1 4
#define ANIM_WALK2 5
#define ANIM_PUNCH 1

extern int exit_game;                   /* flag we'll set to end the game */
extern int x;
extern int y;
extern int counter;
extern int moving;
extern BITMAP *player[11];
extern BITMAP *enemy1[11];
extern BITMAP *bg;
extern int curr_sprite;

extern void input();
extern void process();
extern void output();



#endif
