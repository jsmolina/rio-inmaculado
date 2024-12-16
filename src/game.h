#ifndef GAME_H
#define GAME_H

#include <allegro.h>
#include <allegro/gfx.h>
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

// moving Y states
#define MOVING_UP 7
#define MOVING_DOWN 8

// enemy states
#define ST_NONE 0
#define ST_IDLE 1
#define ST_POSITION_FRONT 2
#define ST_POSITION_BACK 3

#define ANIM_WALK1 4
#define ANIM_WALK2 5
#define ANIM_PUNCH 1
#define ANIM_PUNCH2 2
#define ANIM_HITTED 6
#define HIT_DURATION 6 
#define FLOOR_DURATION 18
// TODO define maximum vertical distance to allow before two objects can no
// longer interact

struct spritePos {
    unsigned int x;
    unsigned int y;
    BITMAP *sprite[11];
    unsigned int moving;
    unsigned int y_moving;
    unsigned int curr_sprite;
    unsigned char is_hit;
    unsigned char is_floor;
    unsigned char lives;
    unsigned char received_hits;
    unsigned char floor_times;
    char is_punching;
};

struct enemyData {
    unsigned int x;
    unsigned int y;
    BITMAP *sprite[11];
    unsigned int moving;
    unsigned int y_moving;
    unsigned targetX;
    unsigned targetY;
    unsigned int curr_sprite;
    char is_hit;
    char is_punching;
    unsigned char received_hits;
    unsigned char floor_times;
};

extern int exit_game; /* flag we'll set to end the game */
extern int level;
extern struct spritePos player;
extern struct enemyData enem1;
extern int counter;
// extern int moving;
// extern BITMAP *player[11];
// extern BITMAP *enemy1[11];
extern BITMAP *bg;

extern void input();
extern void process();
extern void output();
extern void load_curr_level();

#endif

/**
Niveles:
1: clases
2: cours navé
3: sala informática: alleycat piscina
4: parking
5: calle
 */