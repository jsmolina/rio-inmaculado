#include "game.h"
#include "allegro/digi.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "allegro/keyboard.h"
#include "allegro/midi.h"
#include "allegro/text.h"
#include "dat_manager.h"
#include "enem.h"
#include "tiles.h"
#include "helpers.h"
#include "levels.h"
#include "misifu.h"

#include <stdlib.h>
#include <stdio.h>

int exit_game;
spritePos player;

time_t start_playing;
unsigned int score;
unsigned char level = 0;
unsigned char next_level = FALSE;
unsigned char starting_level_counter = 0;
char coursnave_completed = FALSE;
char yellow_key = FALSE;
char blue_key = FALSE;
char locked_elevator;
char urinated;
MIDI *music;
MIDI *final_music;
int counter = 0;
char space_was_pressed = FALSE;
// BITMAP *player[11];
// BITMAP *enemy1[11];
BITMAP *bg;
BITMAP *tiles;
BITMAP *player_head;
BITMAP *girl;
BITMAP *vespino;
BITMAP *key_sprite;
BITMAP *key_sprite_blue;
BITMAP *player_lifebar;
SAMPLE *alleytheme;
SAMPLE *dog_theme;
SAMPLE *hit;
SAMPLE *punch, *punch2;
SAMPLE *fall, *die_sample;
SAMPLE *motorbike, *metalhit;
char slow_cpu;
LevelData levels[TOTAL_LEVELS];

void input() {
    // readkey();
    // end_game = 1;
    if (level == 12 || (level == 8 && !coursnave_completed)) {
        return;
    }
    if (player.moving != LOOKING_WALL) {
        if (player.moving < STOPPOS) {
            if (player.moving == MOVING_LEFT || player.moving == PUNCH_LEFT) {
                player.moving = STOP_LEFT;
            } else {
                player.moving = STOP_RIGHT;
            }
        }

        if (player.y_moving < STOPPOS) {
            player.y_moving = STOPPOS;
        }
    }

    if (player.is_hit > 0 || player.is_floor > 0) {
        return;
    }
    unsigned char pressing_control = key[KEY_RCONTROL] || key[KEY_LCONTROL];
    if (!pressing_control && player.is_punching > 0) {
        player.is_punching = 0;
    }

    // now check again keys
    if (pressing_control) {
        player.is_punching++;
        if (player.is_punching > 20) {
            return;
        }

        if (player.moving == STOP_LEFT) {
            player.moving = PUNCH_LEFT;
        } else {
            player.moving = PUNCH_RIGHT;
        }
    }

    if (key[KEY_LEFT]) {
        player.moving = MOVING_LEFT;
    } else if (key[KEY_RIGHT]) {
        player.moving = MOVING_RIGHT;
    }
    
    if (key[KEY_UP]) {
        player.y_moving = MOVING_UP;
    } else if (key[KEY_DOWN]) {
        player.y_moving = MOVING_DOWN;
    }
    // TODO ALT/ALTGR for kicks
}


void increase_level_and_load() {
    if (level >= 1) {
        return;
    }
    clear_to_color(screen, 0);
    textout_centre_ex(screen, font, "El JONNY te ha robado la vespino.", SCREEN_W / 2,
                      SCREEN_H / 2, makecol(255, 255, 255), -1);
    textout_centre_ex(screen, font, "La JESSI te ha dicho que la tiene", SCREEN_W / 2,
        SCREEN_H / 2 + 40, makecol(255, 255, 255), -1);
    textout_centre_ex(screen, font, "en el parking del insti.", SCREEN_W / 2,
            SCREEN_H / 2 + 60, makecol(255, 255, 255), -1);
    
    textout_centre_ex(screen, font, "SUS AMIGOS TE ESPERAN", SCREEN_W / 2,
        SCREEN_H / 2 + 80, makecol(255, 255, 255), -1);

    draw_sprite(screen, player.sprite[0], 40, 60);
    locked_elevator = TRUE;
    urinated = FALSE;
    beep_count = -1;
    missed_beeps = 0;
    beep_side = IZQUIERDA;
    coursnave_completed = FALSE;
    yellow_key = FALSE;
    blue_key = FALSE;
    score = 0;
    next_level = 1;
    // last level starts with zero enemies
    vespino_enemy.x = 290;
    vespino_enemy.y = 110;
    vespino_enemy.is_floor = FALSE;
    vespino_enemy.direction = VESPINO_HIDDEN;
    vespino_enemy.lifebar = 15;
    player.lives = 3;
    player.win = FALSE;
    player.lifebar = 10;
    player.floor_times = 0;

    start_playing = time(NULL);
    for (int i = 0; i < TOTAL_LEVELS; i++) {
        for (int j=0; j < MAX_ENEMIES; j++) {
            alive_enemies[i][j] = TRUE;
        }        
    }

    while(key[KEY_SPACE]) {
        rest(1);
    }
    int x_moto = 0;
    for (int i = 0; i < 25; i++) {
        x_moto = 88 + 5 * i;
        rectfill(screen, x_moto - 5, 50, x_moto + 54, 99, 0);
        draw_sprite(screen, vespino_enemy.sprite[0], x_moto, 50);
        rest(150);
        vsync();
        if (key[KEY_SPACE]) {
            break;
        } 
    }
    load_level();
}
// draws current player lives
void draw_lives() {
    //blit(bg, screen, 20, 20, 20, 20, 50, 20);
    rectfill(screen, 20, SCREEN_H - 30, 55, SCREEN_H - 20, makecol(40, 40, 40));
    unsigned int i;
    for (i = 0; i < player.lives; i++) {
        draw_sprite(screen, player_head, 20 + i * 10, SCREEN_H - 30);
    }
}

void draw_score() {
    rectfill(screen, 280, SCREEN_H - 30, 319, SCREEN_H - 20, makecol(40, 40, 40));
    textprintf_ex(screen, font, 280, SCREEN_H - 30, makecol(255,255,255), -1, "%05d", score);
}

void game_over() {
    textout_centre_ex(screen, font, "GAME OVER", 190, SCREEN_H - 34, makecol(255, 255, 255), makecol(10, 10, 10));
}

void draw_lifebar() {
    rectfill(screen, 60, SCREEN_H - 30, 88, SCREEN_H - 20, makecol(40, 40, 40));
    blit(player_lifebar, screen, 0, 0, 60, SCREEN_H -30, 2 * player.lifebar, 14);
}

void draw_lifebar_vespino_enemy() {
    textout_ex(screen, font, "JOHNNY", SCREEN_W / 2 - 50, 230,
               makecol(255, 255, 255), -1);
    rectfill(screen, SCREEN_W / 2, 230, SCREEN_W / 2 + 30, 240,
             makecol(40, 40, 40));
    blit(player_lifebar, screen, 0, 0, SCREEN_W / 2, 230,
         2 * vespino_enemy.lifebar, 14);
}

void process() {
    if (level == MISIFU_ALLEY || level ==  MISIFU_CHEESE || level == WIN_LEVEL) {
        misifu_process();
        return;
    }

    move_with_level_limits();


    // check door opening or side moving
    move_to_level_if_needed();

    if (key[KEY_1] && key[KEY_2] && key[KEY_3]) {
        player.win = TRUE;
    }

    if (player.received_hits == HIT_KO) {
        enem_resets();
        player.is_floor = FLOOR_DURATION;
        player.moving = STOPPOS;
        play_sample(fall, 255, 127, 1000, 0);  
        player.received_hits = 0;
    }

    if (player.received_hits == MOTORBIKE_HIT) {
        enem_resets();
        player.is_floor = FLOOR_DURATION / 2;
        player.moving = STOPPOS;
        play_sample(fall, 255, 127, 1000, 0);  
        player.received_hits = 0;
    }

    if (player.lifebar == 0) {
        player.lives--;
        play_sample(die_sample, 255, 127, 1000, 0); 
        player.is_floor = FLOOR_DURATION;
        player.lifebar = LIFEBAR;
        draw_lives();
        draw_lifebar();
    }

    if (player.lives == 0) {
        next_level = GAME_OVER;
        level = GAME_OVER;        
        player.is_floor = FLOOR_DURATION;
        stop_sample(motorbike);
        stop_midi();
        game_over();
        
        return;
    }

    if (player.win == TRUE) {
        level_win();
        return;
    }

    level_processing();
        
    if (next_level != level) {
        load_level();
        return;
    }

    if (player.is_hit > 0) {
        player.curr_sprite = ANIM_HITTED;
    }
    

    if ((counter % 10) == 0) {
        if (player.is_floor > 0) {
            player.is_floor--;
            return;
        }

        if (player.is_hit > 0) {
            player.is_hit--;
            return;
        }

        if (player.moving == MOVING_RIGHT || player.moving == MOVING_LEFT) {
            if ((player.x / MIGUEL_WALK_CYCLE) % 2 == 0) {
                player.curr_sprite = ANIM_WALK2;
            } else {
                player.curr_sprite = ANIM_WALK1;
            }
        } else if (player.y_moving == MOVING_UP || player.y_moving == MOVING_DOWN){
            if (player.curr_sprite == ANIM_WALK1) {
                player.curr_sprite = ANIM_WALK2;
            } else {
                player.curr_sprite = ANIM_WALK1;
            }
        } else if (player.moving == PUNCH_RIGHT ||
                   player.moving == PUNCH_LEFT) {

            if (player.curr_sprite == ANIM_PUNCH) {
                player.curr_sprite = ANIM_PUNCH2;
            } else {
                player.curr_sprite = ANIM_PUNCH;
            }
            // TODO 15/10: sprite de hit
        } else if (player.moving != LOOKING_WALL) {
            player.curr_sprite = 0;
        }
    }
    ///// ENEMY
    all_enemy_decisions();

    if ((counter % 10) == 0) {
        all_enemy_animations();
    }
}

void draw_player() {
    // redraw pair or impair?

    if (player.is_floor != FALSE) {
        if (player.moving & 1) {
            draw_sprite(screen, player.sprite[12], player.x, player.y + 30);
        } else {
            draw_sprite_h_flip(screen, player.sprite[12], player.x, player.y + 30);
        }

    } else {
        if (player.moving & LOOKING_LEFT) {
            draw_sprite_h_flip(screen, player.sprite[player.curr_sprite],
                               player.x, player.y);
        } else {
            draw_sprite(screen, player.sprite[player.curr_sprite], player.x,
                        player.y);
        }
    }
}

typedef struct  {
    unsigned int y;
    int argument;
} yPositions;


int y_comp(const void *a, const void *b) {
    yPositions *elemA = (yPositions *)a;
    yPositions *elemB = (yPositions *)b;
    return (elemA->y - elemB->y);
}

void output() {
    counter++;

    if (level == MISIFU_ALLEY || level ==  MISIFU_CHEESE || level == WIN_LEVEL) {
        return;
    }
    if ((counter % 4) == 0) {
        draw_score();
    }

    if (level == 12) {
        return;
    }
    // clean
    blit(bg, screen, player.x-5, player.y - 10, player.x-5, player.y - 10, 45, 55);
    redraw_bg_enemy_positions();

    if (level == 9 || yellow_key == TRUE) {
        unsigned char y_pos = 105;
        if (yellow_key == TRUE) {
            y_pos = SCREEN_H - 18;
        }
        draw_sprite(screen, key_sprite, 34, y_pos);
    }

    if (blue_key == TRUE) {
        draw_sprite(screen, key_sprite_blue, 64, SCREEN_H - 18);
    }

    // draw in order depending on Y

    yPositions drawn_items[5] = {
        {enemies[0].y,  JOHNY_INDEX},
        {enemies[1].y,  PETER_INDEX},
        {enemies[2].y, ALEX_INDEX},
        {player.y,  PLAYER_INDEX},
        {vespino_enemy.y + 10,  VESPINO_INDEX}
    };
    
    qsort(drawn_items, 5, sizeof(yPositions), y_comp);
    for (int i = 0; i < 5; i++) {
        if (drawn_items[i].argument <= ALEX_INDEX) {
            draw_enemy(drawn_items[i].argument);
        } else if (drawn_items[i].argument == PLAYER_INDEX) {
            draw_player();
        } else if (drawn_items[i].argument == VESPINO_INDEX && level == 11) {
            draw_vespino();
        }
    }

    if(level == 4 && urinated != FALSE) {
        // draw teacher
        draw_sprite(screen, girl, 42, 145);
    }

    if (counter > 319) {
        counter = 0;
    }
}


void init_level_variables(unsigned int initialX, unsigned int initialY) {
    player.x = initialX;
    player.y = initialY;
    player.moving = STOP_RIGHT;
    player.y_moving = 0;
    player.curr_sprite = 0;
    player.is_hit = FALSE;
    player.is_floor = FALSE;
    player.received_hits = 0; // TODO remove
    player.floor_times = 0;
    if (initialY == 130) {
        starting_level_counter = 20; // simulate leaving elevator
    } else {
        starting_level_counter = 5;
    }
    player.curr_sprite = ANIM_WALK1;
    
}

/**
Loads the whole set of levels
*/
void load_levels() {
    FILE* archivo = fopen("levels.csv", "r");
    if (archivo == NULL) {
        die("Cannot open 'levels.csv'\n");
    }
    // discard first line
    char buffer[150];
    fgets(buffer, 72, archivo);
    /*level,door1Pos,door1,door2Pos,door2,left,right,enemies,initialX,initialY,elevatorPos,elevator, minX,maxX */
    int total_levels = 0;
    int level_index = 1;

    while (fscanf(archivo, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 
                  &levels[level_index].level,
                  &levels[level_index].door1Pos,
                  &levels[level_index].door1,
                  &levels[level_index].door2Pos,
                  &levels[level_index].door2,
                  &levels[level_index].left,
                  &levels[level_index].right,
                  &levels[level_index].total_enemies, 
                  &levels[level_index].initialX,
                  &levels[level_index].initialY, 
                  &levels[level_index].elevatorPos,
                  &levels[level_index].elevator,
                  &levels[level_index].minX, 
                  &levels[level_index].maxX) > 2) {
        total_levels++;
        level_index++;
        if (total_levels >= TOTAL_LEVELS) break;
    }

    fclose(archivo);
}



void load_level() {
    unsigned char prev_level = level;
    if (next_level == 6 && prev_level == 5) {
        BITMAP * bg2;

        bg2 = load_level_background(6);
        for (unsigned int i = 0; i < 200; i+=4) {
            blit(bg2, screen, 0, 0, 0, 200 - i, 320, i);
            blit(bg, screen, 0, i, 0, 0, 320, 200 - i);
            vsync();
            // TODO check speed
            rest(100);
        }
        destroy_bitmap(bg2);
    }
    if (bg) {
        destroy_bitmap(bg);
    }

    if (next_level == 0) {
        //bg = load_pcx("bege.pcx", NULL);
        bg = load_level_background(0);
        textout_ex(bg, font, "MSDOS CLUB", SCREEN_H - 20, 40, makecol(100, 100, 100), -1);
        textout_ex(bg, font, "Rio Immaculado", SCREEN_W / 2 - 55, 140, makecol(255, 255, 255), -1);
        textout_ex(bg, font, "Space to start", SCREEN_W / 2 - 40, 80, makecol(156, 176, 239), -1);
        textout_ex(bg, font, "Dedicated to G&C", 70, SCREEN_H - 30, makecol(255, 176, 239), -1);

    } else if (next_level == 12) {
        bg = load_level_background(next_level);
        level = next_level;
    } else if (next_level == MISIFU_ALLEY) {
        bg = load_misifu_alley();
        level = next_level;
    } else if (next_level == MISIFU_CHEESE) {
        bg = load_misifu_cheese();
        level = next_level;
    } else if (next_level >= 1) {
        bg = load_level_background(next_level);
        LevelData curr_leveldata = levels[next_level];
        unsigned int initialX, initialY;
        
        if (next_level < level) { // coming back
            if (is_on_door(levels[level].door1Pos)) {
                initialX = curr_leveldata.door1Pos;
            } else if (is_on_door(levels[level].door2Pos)) {
                initialX = curr_leveldata.door2Pos;
            } else {
                if (point_distance(player.x, levels[level].minX) < 20) {
                    initialX = curr_leveldata.maxX - 15;
                } else {
                    initialX = curr_leveldata.minX + 15;
                }
            }
            initialY = curr_leveldata.initialY;
        } else {
            initialX = curr_leveldata.initialX;
            initialY = curr_leveldata.initialY;
        }

        if (next_level == 10 && blue_key != TRUE) {
            play_sample(alleytheme, 150, 127, 1000, 0);
        }

        level = next_level;
        player.x = initialX;
        player.y = initialY;
        player.moving = STOP_RIGHT;
        player.y_moving = 0;
        player.curr_sprite = 0;
        player.is_hit = FALSE;
        player.is_floor = FALSE;
        player.received_hits = 0; // TODO remove
        player.floor_times = 0;
        if (initialY == 130) {
            starting_level_counter = 20; // simulate leaving elevator
        } else {
            starting_level_counter = 5;
        }
        player.curr_sprite = ANIM_WALK1;

        init_level_enemies();
    }
    if (!bg) {
        die("Cannot load graphic");        
    }
    //blit(bg, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    if (!(level == 6 && prev_level == 5) && level != MISIFU_ALLEY && level != MISIFU_CHEESE) {
        fade_out(16);
    }
    blit(bg, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    if (!(level == 6 && prev_level == 5) && level != MISIFU_ALLEY && level != MISIFU_CHEESE) {
        fade_in(palette, 16);
    }
    
    if (level == 11) {
        draw_lifebar_vespino_enemy();
    }

    //rectfill(screen, 0, 200, 320, 240, makecol(0, 0, 0));
    if (level != 0) {
        draw_lives();
        draw_lifebar();
        draw_score();
    }
}

/**
blit
– Stands for “BLock Transfer”
– Most important graphics function
– Takes a rectangular block of one bitmap and draws it onto
another
**/
