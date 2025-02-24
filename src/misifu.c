#include <stdint.h>
#include <stdio.h>
#include "allegro/color.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "allegro/midi.h"
#include "allegro/platform/astdint.h"
#include "enem.h"
#include "game.h"
#include "misifu.h"
#include "allegro/datafile.h"
#include "tiles.h"


struct clothes {
    unsigned int row1_x;
    unsigned int row2_x;
    BITMAP *sprite1;
    BITMAP *sprite2;
    uint8_t frames_moving;
};
struct bincat {
    uint8_t in_bin;
    int x;
    int y;
    uint8_t appears;
    BITMAP *sprite;
};

struct dog {
    int x;
    BITMAP *sprite[4];
    char appears;
    uint8_t direction;
    uint8_t offset;
};

struct prota {
    struct sp1_ss* sp;
    int x;
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
struct dog dog;
struct clothes clothes;
struct bincat bincat;
struct coord {
    int x;
    int y;
};
PALETTE misifu_palette;
uint8_t random_value;
uint8_t opened_window_frames;
uint8_t opened_window;


BITMAP * load_misifu_data() {
    char file_buffer[16];
    BITMAP *back = load_pcx("alley.pcx", misifu_palette);
    set_pallete(misifu_palette);
    for (int i = 0; i < 7; i++) {
        sprintf(file_buffer, "CAT%d.PCX", i + 1);
        misifu.sprite[i] = load_pcx( file_buffer, NULL );
        if(!misifu.sprite[i]) {
            die("Cannot load %s", file_buffer);
        }
    }
    for (int i = 0; i < 4; i++) {
        sprintf(file_buffer, "DOG%d.PCX", i + 1);
        dog.sprite[i] = load_pcx( file_buffer, NULL );
        if(!dog.sprite[i]) {
            die("Cannot load %s", file_buffer);
        }
    }
    if (music) {
        stop_midi();
        destroy_midi(music);
    }
    music = load_midi("ALLEYCAT.MID");
    play_looped_midi(music, 0, -1);
    clothes.sprite1 = load_pcx("CLOTHES1.PCX", NULL);
    clothes.sprite2 = load_pcx("CLOTHES2.PCX", NULL);
    bincat.in_bin = NONE;
    bincat.appears = NONE;
    bincat.sprite = load_pcx("BINCAT.PCX", NULL);
    
    clothes.row1_x = 225;
    clothes.row2_x = 50;
    clothes.frames_moving = NONE;
    misifu.offset = OFF_BORED;
    dog.offset = OFF_DOG_WALK;
    dog.direction = DOG_LEFT;
    misifu.x = 65;
    misifu.y = FLOOR_Y;
    dog.appears = FALSE;
    opened_window_frames = NONE;
    opened_window = NONE;
        
    return back;
}

static void stop_jump_if_needed(uint8_t max_jump) {
    if ((misifu.initial_jump_y - misifu.y) >= max_jump || misifu.x > 318) {
        misifu.state = M_FALLING;
        misifu.draw_additional = NONE;
    }
}

inline unsigned char is_in_bin() {
    uint8_t bin_positions[] = {29, 60, 93, 182, 215};

    for (int i = 0; i < 6; i++) {
        if (misifu.x >= bin_positions[i] && misifu.x <= (bin_positions[i] + 24)) {
            return i + 1;
        }
    }
    return NONE;
}

inline void check_bincat() {
    // checks if bincat should appear and where
    if (bincat.appears == NONE && misifu.in_bin != NONE) {
        bincat.in_bin = random_value % 12;
        // less probable
        if(bincat.in_bin > 0 && bincat.in_bin < 6) {            
            bincat.appears = 120;
            int bin_pos[5] = {39, 70, 103, 192, 225};
            bincat.x = bin_pos[bincat.in_bin - 1];
            bincat.y = BINCAT_Y_LOW;
            if (bincat.in_bin == HIGHER_BIN1 || bincat.in_bin == HIGHER_BIN2) {
                bincat.y = BINCAT_Y;
            } 
        } else {
            bincat.in_bin = NONE;
        }
    }

    // delete bincat after some frames
    if (bincat.appears != NONE) {
        --bincat.appears;

        // cat falls if cat_in_bin is the same of bincat_in_bin
        if (bincat.in_bin == misifu.in_bin) {
            misifu.state = FALLING_FLOOR;
            misifu.in_bin = NONE;
            //BEEPFX_HIT_2
        }

        if (bincat.appears <= 1) {
            bincat.appears = NONE;
            bincat.in_bin = 0;
            blit(bg, screen, bincat.x, bincat.y, bincat.x, bincat.y, 15, 12);
        }
    }
}

inline void detect_fall_in_bin() {
    // detect falling over bin
    if(misifu.y == 128 || misifu.y == 138) {
        misifu.in_bin = is_in_bin();
        // store that it is on first bin pos so collide will bincat is easier
        if (misifu.in_bin != NONE && misifu.in_bin != bincat.in_bin) {
            if (misifu.y == 128 && (misifu.in_bin == HIGHER_BIN1 || misifu.in_bin == HIGHER_BIN2)) {
                // stop falling
                misifu.state = NONE;
                misifu.draw_additional = CAT_IN_BIN;
            } else if (misifu.y == 138 && misifu.in_bin != HIGHER_BIN1 && misifu.in_bin != HIGHER_BIN2) {
                misifu.state = NONE;
                misifu.draw_additional = CAT_IN_BIN;
            }
        }
    } else if(misifu.y == 100) {
        misifu.state = NONE;
        misifu.draw_additional = CAT_IN_FENCE;
    // now check ropes TODO check ropes clothes are not colliding
    } else if(misifu.y == 75) {
        misifu.state = CAT_IN_ROPE;
        misifu.draw_additional = CAT_IN_ROPE1;
    } else if(misifu.y == 44) {
        misifu.state = CAT_IN_ROPE;
        misifu.draw_additional = CAT_IN_ROPE2;
    } else if(misifu.y == 12) {
        misifu.state = CAT_IN_ROPE;
        misifu.draw_additional = CAT_IN_ROPE3;
    }
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

        if (misifu.y <= 12) {
            misifu.y = 12;
            misifu.offset = OFF_HANGING;
            misifu.state = CAT_IN_ROPE;
            misifu.draw_additional = CAT_IN_ROPE3;
        }
        //}

        stop_jump_if_needed(35);
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
    if(misifu.state == M_FIGHTING) {
        return;
    }

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
    draw_sprite(screen, clothes.sprite2, clothes.row1_x, 50);
    draw_sprite(screen, clothes.sprite1, clothes.row2_x, 82); 
    if (bincat.appears != NONE) {
        draw_sprite(screen, bincat.sprite, bincat.x, bincat.y);
    }
    if (misifu.state == M_WALKING_LEFT || misifu.draw_additional == M_JUMP_LEFT) {
        draw_sprite_h_flip(screen, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    } else {
        draw_sprite(screen, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    }    
    if (dog.appears == TRUE) {
        if (dog.direction == DOG_LEFT) {
            draw_sprite(screen, dog.sprite[dog.offset], dog.x, DOG_Y);
        } else {
            draw_sprite_h_flip(screen, dog.sprite[dog.offset], dog.x, DOG_Y);
        }
    }

}

inline void move_clothes() {
    clothes.row2_x += 3;
    clothes.row1_x -=3;

    if (clothes.row2_x > 225) {
        clothes.row2_x = LEVEL_MIN;
    }
    if (clothes.row1_x <= LEVEL_MIN) {
        clothes.row1_x = 225;
    }

    if((misifu.draw_additional == CAT_IN_ROPE1 || misifu.draw_additional == CAT_IN_ROPE3)
                && misifu.x < LEVEL_MAX) {
        misifu.x += 3;
    } else if(misifu.draw_additional == CAT_IN_ROPE2 && misifu.x > 1) {
        misifu.x -=3;
    }
}

inline void paint_clothes() {
    //clothes.sprite1
    blit(bg, screen, clothes.row1_x, 50, clothes.row1_x, 50, 64, 20);
    blit(bg, screen, clothes.row2_x, 82, clothes.row2_x, 82, 40, 20);
    blit(bg, screen, misifu.x - 2, misifu.y, misifu.x -2, misifu.y, 32, 40);
    blit(bg, screen, dog.x, DOG_Y, dog.x, DOG_Y, 24, 16);

    if ((counter & 1) == 0) {
        if(random_value > 235 && clothes.frames_moving == NONE) {
            clothes.frames_moving = 20;        
        }
        if (clothes.frames_moving != NONE) {
            --clothes.frames_moving;
            move_clothes();
        }
    }
   
}



struct coord get_window_coords() {
    struct coord coordinate;
    coordinate.x = 56 + (opened_window % 4) * 65;
    coordinate.y = 36 + 32 * ((int) (opened_window / 4)) + 15;
    return coordinate;
}

void open_window(uint8_t height, char open_window) {
    struct coord window_coords = get_window_coords();
    
    if (open_window == TRUE) {
        rectfill(screen, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - height, makecol(41, 40, 41));
        rectfill(bg, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - height, makecol(41, 40, 41));
    } else {
        rectfill(screen, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - 8, makecol(86, 255, 255));
        rectfill(bg, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - 8, makecol(86, 255, 255));

        rectfill(screen, window_coords.x, window_coords.y - 8, window_coords.x + 31, window_coords.y - 15, makecol(64, 197, 197));
        rectfill(bg, window_coords.x, window_coords.y - 8, window_coords.x + 31, window_coords.y - 15, makecol(64, 197, 197));
    }
}

inline void anim_windows() {
    if (opened_window_frames == NONE) {
        if (random_value > 125) {
            opened_window = random_value - 125;
        } else {
            opened_window = random_value;
        }
        if (opened_window < 12) { 
            // 0 to 11
            opened_window_frames = 255;
        }
    } else {
        if ((counter & 1) == 0) {
            --opened_window_frames;
        
            if(opened_window_frames == 254) {
                // open half of the window
                struct coord window_coords = get_window_coords();
                // todo do also for bg
                open_window(8, TRUE);                
            } else if (opened_window_frames == 200) {
                open_window(15, TRUE);
            }
            if (opened_window_frames == 0) {
                open_window(15, FALSE);
            }
        }
    }
}

inline void dog_checks() {
    if (dog.appears == TRUE) {
        if ((counter & 1) == 0) {
            if (misifu.state != M_FIGHTING) {
                if (dog.direction == DOG_LEFT) {
                    dog.x -= 2;
                } else {
                    dog.x += 3;
                }

                if ((dog.x / WALK_CYCLE) % 2 == 0) {
                    dog.offset = OFF_DOG_WALK + 1;
                } else {
                    dog.offset = OFF_DOG_WALK;
                }

                // TODO if cat on floor, change direction!
                if (misifu.y >= 160) {
                    if (misifu.x > dog.x) {
                        dog.direction = DOG_RIGHT;
                    } else {
                        dog.direction = DOG_LEFT;
                    }

                    if (abs(misifu.x - dog.x) <= 4) {
                        misifu.state = M_FIGHTING;
                        dog.offset = OFF_DOG_FIGHT;
                    }
                } else if(dog.direction == DOG_RIGHT) {
                    dog.direction = DOG_LEFT;
                }

            } else {
                // he is fighting
                if ((dog.x / WALK_CYCLE) % 2 == 0) {
                    dog.offset = OFF_DOG_FIGHT + 1;
                } else {
                    dog.offset = OFF_DOG_FIGHT;
                }
                dog.x -=3;
                misifu.x = dog.x;
            }
        } 

        if (dog.x <= LEVEL_MIN) {
            dog.appears = FALSE;
            if (misifu.state == M_FIGHTING) {
                misifu.state = FALLING_FLOOR;
            }
        }
    }

    if (dog.appears != TRUE && (counter & 1) == 0) {
        if (random_value > 253) {
            dog.appears = TRUE;
            dog.x = LEVEL_MAX;
        }
    }
}

inline void alley_loop() {
    char buff[15];
    sprintf(buff, "x:%d,y:%d", misifu.x, misifu.y);
    textout_ex(screen, font, "              ", 0, 0, makecol(255, 255, 255), makecol(0, 0, 0));    
    textout_ex(screen, font, buff, 0, 0, makecol(255, 255, 255), makecol(0, 0, 0));    

    if((counter & 1) == 0) {
        paint_clothes();
        /*paint_bricks(1);
        paint_clothes(1);
        paint_clothes(0);*/
        // now move cat
        if(misifu.state == CAT_IN_ROPE) {
            /*if((misifu.draw_additional == CAT_IN_ROPE1 || misifu.draw_additional == CAT_IN_ROPE3)
                && misifu.x < LEVEL_MAX) {
                 misifu.x += 3;
            } else if(misifu.draw_additional == CAT_IN_ROPE2 && misifu.x > 1) {
                misifu.x -=3;
            }*/
            if(misifu.x >= LEVEL_MAX || misifu.x <= LEVEL_MIN) {
                misifu.state = FALLING_FLOOR;
                misifu.draw_additional = NONE;
                ++misifu.y;
            }
        }
    }
    
    check_bincat();
    dog_checks();
    
    if (misifu.state == M_FALLING) {
        detect_fall_in_bin();
    }
    if (misifu.draw_additional == CAT_IN_BIN && misifu.y < FLOOR_Y && misifu.in_bin != NONE) {
        if (is_in_bin() == NONE) {
            misifu.state = M_FALLING;
            misifu.draw_additional = NONE;
            misifu.in_bin = NONE;
        }
    }
    //detect_fall_in_window();
}

void misifu_process() {
    random_value = rand() % 255;
    check_keys();
    alley_loop();
    anim_windows();
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
    if (music) {
        stop_midi();
        destroy_midi(music);
        music = load_midi("ROGERR.MID");
        play_looped_midi(music, 0, -1);
    }
    set_palette(palette);
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