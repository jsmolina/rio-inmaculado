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
#define LOOKING_WALL 54
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
#define ANIM_ESPALDA 11
#define HIT_DURATION 4
#define HIT_DURATION_ENEM 2
#define FLOOR_DURATION 18

#define TOTAL_LEVELS 12



void beep(int frequency, int duration);
int random_range(unsigned int low, unsigned int high);
int point_distance(unsigned int x, unsigned int targetX);
char is_cpu_slow();

typedef struct  {
    unsigned int x;
    unsigned int y;
    BITMAP *sprite[13];
    unsigned int moving;
    unsigned int y_moving;
    unsigned int curr_sprite;
    int is_hit;
    int is_floor;
    unsigned char lives;
    unsigned char lifebar;
    unsigned char received_hits;
    unsigned char floor_times;
    int is_punching;
} spritePos;

typedef struct  {
    unsigned int x;
    unsigned int y;
    BITMAP *sprite[12];
    unsigned int moving;
    unsigned int y_moving;
    unsigned int targetX;
    unsigned int targetY;
    unsigned int curr_sprite;
    int is_hit;
    int is_punching;
    unsigned char received_hits;
    int is_floor;
    unsigned char floor_times;
    int counter_for_state;
    int variant; // which enemy are we painting?
} enemyData;

typedef struct {
    int x;
    int y;
    BITMAP *sprite[2];
    unsigned int received_hits;
    int is_floor;
    int hit;
    uint8_t direction;
} vespinoData;

typedef struct  {
    // TODO remove, just for helping
    unsigned int level; 
    
    unsigned int door1Pos;
    unsigned int door1; 

    unsigned int door2Pos;
    unsigned int door2;

    unsigned int left; 
    unsigned int right;

    int total_enemies;

    unsigned int initialX;
    unsigned int initialY;
    unsigned int elevatorPos;
    unsigned int elevator;
    unsigned int minX;
    unsigned int maxX;
} LevelData;

/* 
level,door1Pos,door1,door2Pos,door2,left,right,enemies,initialX,initialY,elevatorPos,elevator
*/

void die(const char *format, ...);
#endif
