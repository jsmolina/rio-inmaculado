#include "misifu.h"
#include <stdio.h>
#include "game.h"

struct prota {
    struct sp1_ss* sp;
    unsigned int x;
    unsigned int y;
    unsigned int initial_jump_y;
    uint8_t draw_additional;
    unsigned int  offset;
    uint8_t in_bin;
    uint8_t state;
    int counter_for_state;
    uint8_t last_key;
    BITMAP *sprite[8];
};
struct prota misifu;

static void stop_jump_if_needed(uint8_t max_jump) {
    if ((misifu.initial_jump_y - misifu.y) >= max_jump || misifu.x > 318) {
        misifu.state = M_FALLING;
        misifu.draw_additional = NONE;
    }
}

BITMAP * load_misifu_data() {
    char file_buffer[16];
    BITMAP *back = load_pcx("alley.pcx", NULL);
    for (int i = 0; i < 7; i++) {
        sprintf(file_buffer, "CAT%d.PCX", i + 1);
        misifu.sprite[i] = load_pcx( file_buffer, NULL );
        if(!misifu.sprite[i]) {
            die("Cannot load %s", file_buffer);
        }
    }
    misifu.offset = OFF_BORED;
    misifu.x = 65;
    misifu.y = FLOOR_Y;
    
    return back;
}

void check_fsm() {
    // decide new FSM draw status
    if ((misifu.state == NONE) && abs(counter - misifu.counter_for_state) > 10) {
        misifu.offset = OFF_BORED;
        //misifu.initial_jump_x = 0;
    } else if (misifu.state == M_WALKING_RIGHT || misifu.state == M_WALKING_LEFT) {
        if ((misifu.x / WALK_CYCLE) % 2 == 0) {
            misifu.offset = OFF_RIGHTC1;
        } else {
            misifu.offset = OFF_RIGHTC2;
        }
        misifu.state = NONE;
    } else if (misifu.state == M_JUMPING_PUSHED){
        if(misifu.y > 2) {
            misifu.y = misifu.y - 2;
        } else {
            misifu.state = M_FALLING;
        }
        if (misifu.draw_additional == M_JUMP_LEFT) {
            if(misifu.x > (LEVEL_MIN + 1)) {
                misifu.x = misifu.x - 2;
            }
        } else {
            if(misifu.x < (LEVEL_MAX - 2)) {
                misifu.x = misifu.x + 2;
            }
        }
        stop_jump_if_needed(10);
    } else if (misifu.state == M_JUMPING) {
        misifu.y -= 2;

        if(misifu.draw_additional == M_JUMP_RIGHT && misifu.x < LEVEL_MAX) {

            misifu.offset = OFF_JRIGHT;
            if((key[KEY_RIGHT])) {
                ++misifu.x;
            }
        }  else if(misifu.draw_additional == M_JUMP_LEFT && misifu.x > LEVEL_MIN) {
            misifu.offset = OFF_JRIGHT;

            if((key[KEY_LEFT])) {
                --misifu.x;
            }
        } else {
            misifu.offset = OFF_JUMPING;
        }

        //if (level == 1) {
        if (misifu.y <= 1) {
            misifu.y = 2;
            misifu.offset = OFF_HANGING;
            misifu.state = CAT_IN_ROPE;
            misifu.draw_additional = CAT_IN_ROPE3;
        }
        //}

        stop_jump_if_needed(30);
    } else if (misifu.state == M_FALLING) {
        ++misifu.y;
        misifu.offset = OFF_FALL;

        if(misifu.y >= FLOOR_Y) {
            misifu.y = FLOOR_Y;
            misifu.state = NONE;
            misifu.offset = OFF_BORED;
            //misifu.initial_jump_x = 0;
        }
    } else if (misifu.state == FALLING_FLOOR) {
        misifu.y += 2;
        misifu.offset = OFF_FALL;
        misifu.draw_additional = NONE;
        if(misifu.y >= FLOOR_Y) {
            misifu.y = FLOOR_Y;
            misifu.state = NONE;
        }
    } else if(misifu.state == CAT_IN_ROPE) {
        misifu.offset = OFF_HANGING;

    }
}

void check_keys() {
    if((!(key[KEY_UP]) && !(key[KEY_DOWN]))) {
        misifu.last_key = NONE;
    }
    if ((misifu.last_key != KEY_UP) && (key[KEY_UP]) && (misifu.y > 0) && (misifu.state == NONE || misifu.state == M_WALKING_LEFT || misifu.state == M_WALKING_RIGHT || misifu.state == CAT_IN_ROPE || misifu.state ==CAT_ON_HIGH) ) {
        misifu.state = M_JUMPING;
        misifu.in_bin = NONE;
        misifu.initial_jump_y = misifu.y;
        misifu.last_key = KEY_UP;
        misifu.counter_for_state = counter;

        if((key[KEY_RIGHT]) && misifu.x < LEVEL_MAX ) {
            misifu.draw_additional = M_JUMP_RIGHT;
        } else if((key[KEY_LEFT]) && misifu.x > 0 ) {
            misifu.draw_additional = M_JUMP_LEFT;
        } else {
            misifu.draw_additional = M_JUMP_UP;
        }
    } else if ((key[KEY_RIGHT])  && misifu.x < LEVEL_MAX && (misifu.state == NONE || misifu.state == M_WALKING_LEFT || misifu.state == M_WALKING_RIGHT|| misifu.state == CAT_ON_HIGH)) {
        misifu.state = M_WALKING_RIGHT;
        misifu.counter_for_state = counter;
        ++misifu.x;
    } else if((key[KEY_LEFT])  && misifu.x >= LEVEL_MIN && (misifu.state == NONE || misifu.state == M_WALKING_LEFT || misifu.state == M_WALKING_RIGHT|| misifu.state == CAT_ON_HIGH)) {
        misifu.state = M_WALKING_LEFT;
        misifu.counter_for_state = counter;
        --misifu.x;
    } else if ((misifu.last_key != KEY_DOWN) && (key[KEY_DOWN]) && misifu.y < FLOOR_Y) {
        misifu.last_key = KEY_DOWN;
        misifu.state = M_FALLING;
        misifu.in_bin = NONE;
        ++misifu.y;
    }
}

void misifu_output() {
    blit(bg, screen, misifu.x, misifu.y - 10, misifu.x, misifu.y - 10, 30, 50);

    if (misifu.state == M_WALKING_LEFT || misifu.draw_additional == M_JUMP_LEFT) {
        draw_sprite_h_flip(screen, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    } else {
        draw_sprite(screen, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    }
}

void misifu_process() {
    check_keys();
    misifu_output();
    check_fsm();

    /* 
    level1_loop();
            // cat falls appart from bin
            if (misifu.draw_additional == CAT_IN_BIN && misifu.y < FLOOR_Y && misifu.in_bin != NONE) {
                if (is_in_bin(misifu.x) == NONE) {
                    misifu.state = FALLING;
                    misifu.draw_additional = NONE;
                    misifu.in_bin = NONE;
                }
            }
    */

}


void destroy_misifu_data() {
    // todo destroy sprite
    for (int i = 0; i < 8; i++) {
        destroy_bitmap(misifu.sprite[i]);
    }
}

/*
if (misifu.draw_additional == CAT_IN_BIN && misifu.y < FLOOR_Y && misifu.in_bin != NONE) {
    if (is_in_bin(misifu.x) == NONE) {
        misifu.state = FALLING;
        misifu.draw_additional = NONE;
        misifu.in_bin = NONE;
    }
}
*/