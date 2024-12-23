#ifndef HELPERS_H
#define HELPERS_H

#include <allegro.h>
#include <allegro/gfx.h>

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

#define LOOKING_LEFT 1

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

#define TOTAL_LEVELS 2


extern int random_range(unsigned int low, unsigned int high);
extern int point_distance(unsigned int x, unsigned int targetX);
extern char is_cpu_slow();

typedef struct  {
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
} spritePos;

typedef struct  {
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
    char is_active;
    unsigned char variant; // which enemy are we painting?
} enemyData;

#endif
