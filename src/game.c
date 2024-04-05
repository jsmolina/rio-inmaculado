#include "game.h"
#include "allegro/inline/draw.inl"
#include "allegro/keyboard.h"
#include <math.h>

int exit_game;
int x;
int y;
int moving = STOP_RIGHT;
int curr_sprite = 0;
int counter = 0;
BITMAP *player[11];
BITMAP *playeri[11];
BITMAP *bg;
BITMAP *tiles;

void input() {
    // readkey();
    // end_game = 1;
    if (key[KEY_ESC]) {
        exit_game = 1;
    }
    if (moving < STOPPOS) {
        if (moving == MOVING_LEFT || moving == PUNCH_LEFT) {
            moving = STOP_LEFT;
        } else {
            moving = STOP_RIGHT;
        }
    }
    if (key[KEY_SPACE]) {
        if (moving == STOP_LEFT) {
            moving = PUNCH_LEFT;
        } else {
            moving = PUNCH_RIGHT;
        }
    } else if (key[KEY_LEFT]) {
        moving = MOVING_LEFT;
    } else if (key[KEY_RIGHT]) {
        moving = MOVING_RIGHT;
    }
}

void process() {
    // delete sprite
    blit(bg, screen, x, y, x, y, 40, 40);
    if (moving == MOVING_RIGHT && x < 300) {
        x++;
    }
    if (moving == MOVING_LEFT && x > 1) {
        x--;
    }
}

void output() {
    counter++;
    // circlefill (screen, x - 1, y, 5, TRANS); /* erase from last place */
    // circlefill (screen, x, y, 5, 15);        /* draw first time */

    if ((counter % 10) == 0) {
        if (moving == MOVING_RIGHT || moving == MOVING_LEFT) {
            if (curr_sprite == ANIM_WALK1) {
                curr_sprite = ANIM_WALK2;
            } else {
                curr_sprite = ANIM_WALK1;
            }
        } else if (moving == PUNCH_RIGHT || moving == PUNCH_LEFT) {
            curr_sprite = ANIM_PUNCH;
        } else {
            curr_sprite = 0;
        }
    }
    // redraw pair or impair?
    if (moving & 1) {
        draw_sprite_h_flip(screen, player[curr_sprite], x, y);
    } else {
        draw_sprite(screen, player[curr_sprite], x, y);
        // draw_sprite_h_flip(screen, player[curr_sprite], x, y);
    }

    if (counter > 319) {
        counter = 0;
    }
}


/**
blit
– Stands for “BLock Transfer”
– Most important graphics function
– Takes a rectangular block of one bitmap and draws it onto
another
**/
