#include <stdint.h>
#include <stdio.h>
#include "allegro/color.h"
#include "allegro/digi.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "allegro/inline/gfx.inl"
#include "allegro/keyboard.h"
#include "allegro/midi.h"
#include "allegro/platform/astdint.h"
#include "allegro/text.h"
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
struct holes {
    int x;
    int y;
};

struct object {
    int x;
    int y;
    char appears;
    BITMAP *sprite;
    uint8_t direction;
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
    int y;
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
struct object object;

struct coord {
    int x;
    int y;
};
PALETTE misifu_palette;
uint8_t random_value;
uint8_t opened_window_frames;
uint8_t opened_window;
uint8_t exit_misifu;
unsigned char original_lives;
BITMAP *heart;

struct holes holes_pos[HOLES_POS] = {
    25,140, 
    42,122, 
    52,95, 
    90, 132, 
    156, 138, 
    139, 117,
    81, 98,
    139, 95,
    162, 84,
    66,77,
    31, 83,
    26, 50,
    58, 42,
    74, 21,
    97,21,
    115, 31
};

struct holes holes_conn[HOLES_CONN] = {
    1, 6,
    4, 8,
    9, 12,
    3, 5,
};

BITMAP *load_misifu_alley() {
    char file_buffer[16];

    stop_sample(alleytheme);
    BITMAP *back = load_pcx("alley.pcx", misifu_palette);
    heart = load_pcx("HEART.PCX", NULL);
    original_lives = player.lives;
    set_palette(misifu_palette);
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
    object.sprite = load_pcx("PHONE.PCX", NULL);
    object.direction = NONE;
    exit_misifu = 0;
    clothes.row1_x = 225;
    clothes.row2_x = 50;
    clothes.frames_moving = NONE;
    misifu.offset = OFF_BORED;
    dog.offset = OFF_DOG_WALK;
    dog.direction = MDIR_LEFT;
    misifu.x = 65;
    misifu.y = FLOOR_Y;
    misifu.state = FALLING_FLOOR;
    dog.appears = FALSE;
    opened_window_frames = NONE;
    opened_window = NONE;
        
    return back;
}

BITMAP * load_misifu_cheese() {
    BITMAP *back = load_pcx("cheese.pcx", misifu_palette);
    opened_window_frames = NONE;
    opened_window = NONE;
    misifu.y = FLOOR_Y;
    dog.appears = FALSE;
    dog.offset = OFF_DOG_WALK;
    dog.direction = MDIR_LEFT;
    bincat.in_bin = NONE;
    bincat.appears = NONE;
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
            play_sample(hit, 200, 127, 1000, 0);
            misifu.state = FALLING_FLOOR;
            misifu.in_bin = NONE;
            //BEEPFX_HIT_2
        }

        if (bincat.appears <= 1) {
            bincat.appears = NONE;
            bincat.in_bin = 0;
            blit(bg, double_buffer, bincat.x, bincat.y, bincat.x, bincat.y, 15, 12);
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
                misifu.x += 1;
            }
        }  else if(misifu.draw_additional == M_JUMP_LEFT && misifu.x > LEVEL_MIN) {
            misifu.offset = OFF_JRIGHT;

            if((key[KEY_LEFT])) {
                misifu.x -=1;
            }
        } else {
            misifu.offset = OFF_JUMPING;
        }

        /*if (misifu.y <= 12) {
            misifu.y = 12;
            misifu.offset = OFF_HANGING;
            misifu.state = CAT_IN_ROPE;
            misifu.draw_additional = CAT_IN_ROPE3;
        }*/
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
    if ((misifu.last_key != KEY_UP) && (key[KEY_UP]) && (misifu.y > 0) && (misifu.state == NONE || misifu.state == M_WALKING_LEFT || misifu.state == M_WALKING_RIGHT || misifu.state == CAT_IN_ROPE || misifu.state == CAT_ON_HIGH) ) {
        misifu.state = M_JUMPING;
        misifu.in_bin = NONE;
        misifu.initial_jump_y = misifu.y;
        misifu.last_key = KEY_UP;
        misifu.counter_for_state = counter;

        if((key[KEY_RIGHT]) && misifu.x < LEVEL_MAX ) {
            misifu.draw_additional = M_JUMP_RIGHT;
        } else if((key[KEY_LEFT]) && misifu.x > LEVEL_MIN ) {
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

    if (key[KEY_ESC]) {
        exit_misifu = MEXIT_END;
    }
    if (key[KEY_1]) {
        next_level = MISIFU_CHEESE;
    }
}

void misifu_clean() {
    if (level == MISIFU_ALLEY) {
        blit(bg, double_buffer, clothes.row1_x, 50, clothes.row1_x, 50, 64, 20);
        blit(bg, double_buffer, clothes.row2_x, 82, clothes.row2_x, 82, 40, 20);
        blit(bg, double_buffer, object.x, object.y, object.x, object.y, 16, 14);

    }
    blit(bg, double_buffer, misifu.x - 2, misifu.y -2, misifu.x - 2, misifu.y - 2, 32, 42);
    blit(bg, double_buffer, dog.x - 2, DOG_Y, dog.x - 2, DOG_Y, 30, 16);
}

void misifu_output() {   
    if (level == MISIFU_ALLEY) {
        draw_sprite(double_buffer, clothes.sprite2, clothes.row1_x, 50);
        draw_sprite(double_buffer, clothes.sprite1, clothes.row2_x, 82);
    }
    if (bincat.appears != NONE) {
        draw_sprite(double_buffer, bincat.sprite, bincat.x, bincat.y);
    }
    if (misifu.state == M_WALKING_LEFT || misifu.draw_additional == M_JUMP_LEFT) {
        draw_sprite_h_flip(double_buffer, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    } else {
        draw_sprite(double_buffer, misifu.sprite[misifu.offset],
            misifu.x, misifu.y);
    }    
    if (dog.appears == TRUE) {
        if (dog.direction == MDIR_LEFT) {
            draw_sprite(double_buffer, dog.sprite[dog.offset], dog.x, DOG_Y);
        } else {
            draw_sprite_h_flip(double_buffer, dog.sprite[dog.offset], dog.x, DOG_Y);
        }
    }
    if (object.direction != NONE) {
        draw_sprite(double_buffer, object.sprite, object.x, object.y);
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
        rectfill(double_buffer, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - height, makecol(41, 40, 41));
        rectfill(bg, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - height, makecol(41, 40, 41));
        if (height == 15 && misifu.y <= 100) {
            object.appears = TRUE;
            object.x = window_coords.x;
            object.y = window_coords.y - 15;

            if(misifu.x < object.x && (object.x - misifu.x) > 2) {
                object.direction = MDIR_LEFT;
            } else if(misifu.x > object.x && ( misifu.x - object.x) > 2) {
                object.direction = MDIR_RIGHT;
            }

            if(misifu.y < object.y && (object.y - misifu.y) > 2) {
                object.direction |= MDIR_UP;
            } else if(misifu.y > object.y && (misifu.y - object.y) > 2) {
                object.direction |= MDIR_DOWN;
            }
        }
    } else {
        rectfill(double_buffer, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - 8, makecol(86, 255, 255));
        rectfill(bg, window_coords.x, window_coords.y, window_coords.x + 31, window_coords.y - 8, makecol(86, 255, 255));

        rectfill(double_buffer, window_coords.x, window_coords.y - 8, window_coords.x + 31, window_coords.y - 15, makecol(64, 197, 197));
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
            opened_window_frames = 90;
        }
    } else {
        struct coord window_coords = get_window_coords();
        if (misifu.state == M_FALLING || misifu.state == M_JUMPING) {
            int normalized_window_y = window_coords.y - 15;
            if (misifu.initial_jump_y <= 100 && misifu.y >= normalized_window_y && misifu.y <= (normalized_window_y + 24) && misifu.x >= window_coords.x && misifu.x <= (window_coords.x+31)) {
                next_level = MISIFU_CHEESE;
                return;
            }
        }
        if ((counter & 1) == 0) {
            --opened_window_frames;
        
            if(opened_window_frames == 88) {
                open_window(8, TRUE);                
            } else if (opened_window_frames == 80) {
                open_window(15, TRUE);
            }

            if (opened_window_frames <= 1) {
                open_window(15, FALSE);
                object.direction = NONE;
            }

            if (object.direction != NONE) {

                if(misifu.state != FALLING_FLOOR && misifu.y <= 100 && abs(misifu.x - object.x) < 8 && abs(misifu.y - object.y) < 16) {
                    // sound zap and icon zap
                    misifu.state = FALLING_FLOOR;
                    play_sample(hit, 200, 127, 1200, 0);
                } else {
                    if (object.y > 140 || object.y < 2 || object.x > LEVEL_MAX || object.x < LEVEL_MIN ) {
                        object.direction = NONE;
                        return;
                    }

                    // now move accordingly
                    if ((object.direction & MDIR_LEFT) != 0) {
                        object.x -= 4;
                    } else if ((object.direction & MDIR_RIGHT) != 0){
                        object.x += 4;
                    }
    
                    if ((object.direction & MDIR_UP) != 0) {
                        object.y -= 4;
                    } else if ((object.direction & MDIR_DOWN) != 0) {
                        object.y += 4;
                    }
                    
                }
                
            }

        }
    }
}

inline void dog_checks() {
    if (dog.appears == TRUE) {
        if ((counter & 1) == 0) {
            if (misifu.state != M_FIGHTING) {
                if (dog.direction == MDIR_LEFT) {
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
                        dog.direction = MDIR_RIGHT;
                    } else {
                        dog.direction = MDIR_LEFT;
                        --dog.x;
                    }

                    if (abs(misifu.x - dog.x) <= 4) {
                        misifu.state = M_FIGHTING;
                        dog.offset = OFF_DOG_FIGHT;
                    }
                } else if(dog.direction == MDIR_RIGHT) {
                    dog.direction = MDIR_LEFT;
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
            stop_sample(dog_theme);
            
            if (misifu.state == M_FIGHTING) {
                stop_sample(dog_theme);
                misifu.state = FALLING_FLOOR;
                player.lives--;
                draw_lives();
            }
        }
    }

    if (dog.appears != TRUE && (counter & 1) == 0) {
        if (random_value > 250) {
            play_sample(dog_theme, 50, 127, 1000, 1);
            dog.appears = TRUE;
            dog.x = LEVEL_MAX;
        }
    }
}

inline void alley_loop() {
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

void destroy_misifu_data() {
    // todo destroy sprite
    for (int i = 0; i < 8; i++) {
        destroy_bitmap(misifu.sprite[i]);
    }
    for (int i = 0; i < 4; i++) {
        destroy_bitmap(dog.sprite[i]);
    }
    destroy_bitmap(clothes.sprite1);
    destroy_bitmap(clothes.sprite2);
    destroy_bitmap(bincat.sprite);
    destroy_bitmap(object.sprite);
    destroy_bitmap(heart);

    if (music) {
        stop_midi();
        destroy_midi(music);
        music = load_midi("ROGERR.MID");
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



void misifu_process() {
    random_value = rand() % 255;

    check_keys();
    if (level == MISIFU_ALLEY) {
        misifu_clean();
        alley_loop();
        dog_checks();
        anim_windows();
    } else if (level == MISIFU_CHEESE) {
        // cheez loop
        dog_checks();
        misifu_clean();
        for (int h = 0; h < HOLES_POS; h++) {
            if (misifu.state == M_FALLING) {
                int right_x = (holes_pos[h].x + 6);
                int down_y = (holes_pos[h].y + 4);
                if(misifu.x >= holes_pos[h].x && misifu.x <= right_x && misifu.y >= holes_pos[h].y && misifu.y <= down_y) {
                    misifu.state = CAT_IN_ROPE;
                    misifu.draw_additional = h;
                    misifu.x = holes_pos[h].x;
                    misifu.y = holes_pos[h].y;
                    
                    if (misifu.x == 26 && misifu.y == 50) {
                        blue_key = TRUE;                
                        exit_misifu = MEXIT_SUCCESS;
                    }                
                }
            }
        }
        if (key[KEY_SPACE] && misifu.state == CAT_IN_ROPE)  {
            for (int ii = 0; ii < HOLES_CONN; ii++) {
                if (misifu.draw_additional == holes_conn[ii].x) {
                    int new_hole = holes_conn[ii].y;
                    misifu.draw_additional = new_hole;
                    misifu.x = holes_pos[new_hole].x;
                    misifu.y = holes_pos[new_hole].y;
                }
            }
        }
    }
    
    if (next_level == level) {
        misifu_output();
        check_fsm();
    } else {
        stop_sample(dog_theme);
        load_level();
    }

    if (player.lives == 0) {
        exit_misifu = MEXIT_END;
    }
    if (exit_misifu == MEXIT_SUCCESS) {
        stop_sample(dog_theme);
        stop_midi();
        clear_to_color(screen, 0);
        for (int i = 0; i < 6; i++) {
            exit_misifu = MEXIT_END;
            draw_sprite(screen, heart, 82 , 170 - 24 * i);
            draw_sprite(screen, heart, 110, 170 - 24 * i);
            draw_sprite(screen, heart, 225, 170 - 24 * i);
            draw_sprite(screen, heart, 253, 170 - 24 * i);
            rest(100);
            vsync();
        }
        rest(1000);
    }

    if (exit_misifu == MEXIT_END) {
        stop_sample(dog_theme);
        destroy_misifu_data();
        next_level = 10;
        player.lives = original_lives;

        FILE *file;
        char line[61];
        file = fopen("shareware.txt", "r");
        if (!file) {
            die("Cannot open file shareware.txt");
        }
        set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }

        printf("\nPress ENTER to continue\n");
        printf("T:\\NOVELL\\MISIFU>exit");
        while (!key[KEY_ENTER] && !key[KEY_SPACE] && !key[KEY_ESC]) {
            rest(1);
        }

        set_color_depth(8);
        if(set_gfx_mode(GFX_MODEX, 320, 240, 0, 0) != 0) {
            die("error setting 320x240 16bpp: %s", allegro_error);
        }        
        set_palette(palette);
        play_looped_midi(music, 0, -1);

        // WARN! After a gfx mode set, requires setting video bitmap again
        double_buffer = create_video_bitmap(SCREEN_W, SCREEN_H);
        bg_video = create_video_bitmap(SCREEN_W, SCREEN_H);
        return;
    }
}
