#include "game.h"
#include "allegro/inline/draw.inl"
#include "allegro/keyboard.h"
#include "dat_manager.h"
#include "enem.h"
#include "tiles.h"

#include "helpers.h"
#include <stdlib.h>

int exit_game;
struct spritePos player;

int level = 0;
int level_enemies = 0;
int counter = 0;
char space_was_pressed = FALSE;
// BITMAP *player[11];
// BITMAP *enemy1[11];
BITMAP *bg;
BITMAP *tiles;
char slow_cpu;

void input() {
    // readkey();
    // end_game = 1;

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

    if (player.is_hit > 0 || player.is_floor > 0) {
        return;
    }
    if (!key[KEY_SPACE] && player.is_punching > 0) {
        player.is_punching = 0;
    }

    // now check again keys
    if (key[KEY_SPACE]) {
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
}

void process() {
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-maker-part-1-player-movement-attacks-and-basic-enemies--cms-26147t
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-maker-part-2-combat-and-basic-enemy-ai--cms-26148t
    // https://code.tutsplus.com/building-a-beat-em-up-in-game-makercombo-attacks-more-ai-and-health-pickups--cms-26471t
    // delete sprite
    if (player.moving == MOVING_RIGHT && player.x < 300) {
        player.x++;
    } else if (player.moving == MOVING_LEFT && player.x > 1) {
        player.x--;
    }

    if (player.y_moving == MOVING_DOWN && player.y < 151) {
        player.y++;
    } else if (player.y_moving == MOVING_UP && player.y > 140) {
        player.y--;
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

        if (player.moving == MOVING_RIGHT || player.moving == MOVING_LEFT ||
            player.y_moving == MOVING_UP || player.y_moving == MOVING_DOWN) {
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
        } else {
            player.curr_sprite = 0;
        }
    }
    ///// ENEMY
    all_enemy_decisions(&player);

    if ((counter % 10) == 0) {
        all_enemy_animations();
    }
}

void draw_player() {
    // redraw pair or impair?
    if (player.is_floor > 0) {
        if (player.moving & 1) {
            rotate_sprite(screen, player.sprite[0], player.x, player.y + 10,
                          itofix(1 * 64));
        } else {
            rotate_sprite_v_flip(screen, player.sprite[0], player.x,
                                 player.y + 10, itofix(1 * 64));
        }

    } else {
        if (player.moving & 1) {
            draw_sprite_h_flip(screen, player.sprite[player.curr_sprite],
                               player.x, player.y);
        } else {
            draw_sprite(screen, player.sprite[player.curr_sprite], player.x,
                        player.y);
        }
    }
}

void output() {
    counter++;
    // clean
    blit(bg, screen, player.x, 120, player.x, 120, 40, 80);
    redraw_bg_enemy_positions();

    // draw in order depending on Y
    if (player_over_all_enemies(player.y)) {
        draw_player();
        all_draw_enemies();
    } else {
        all_draw_enemies();
        draw_player();
    }

    if (counter > 319) {
        counter = 0;
    }
}


int show_bg() {
   BITMAP *bmp, *buffer;
   PALETTE pal;
   int alpha;
   
   if (!bg)
      return -1;

   buffer = create_bitmap(SCREEN_W, SCREEN_H);
   blit(screen, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   set_palette(pal);

   /* fade it in on top of the previous picture */
   for (alpha=0; alpha<256; alpha+=8) {
      set_trans_blender(0, 0, 0, alpha);
      draw_trans_sprite(buffer, bg,
			(SCREEN_W-bg->w)/2, (SCREEN_H-bg->h)/2);
      vsync();
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);      
   }

   blit(bg, screen, 0, 0, (SCREEN_W-bg->w)/2, (SCREEN_H-bg->h)/2,
	bg->w, bg->h);

   destroy_bitmap(buffer);
}


void load_level(int lvl) {
    if (lvl == 0) {
        bg = load_pcx("bege.pcx", NULL);
        level_enemies = 0;
    } else if (lvl == 1) {
        load_tiles();
        bg = load_background("bg4_0.tmx");
        level_enemies = 1;
    }
    if (!bg) {
        allegro_message("Cannot load graphic");
        exit(1);
    }
    if (slow_cpu) {
        blit(bg, screen, 0, 0, 0, 0, 320, 200);
    } else {
        show_bg();
    }
    //

    init_level_enemies(level_enemies, FALSE);

    level = lvl;
}

void increase_level_and_load() {
    if (level >= 1) {
        return;
    }
    clear_to_color(screen, 0);
    textout_centre_ex(screen, font, "Loading Level...", SCREEN_W / 2,
                      SCREEN_H / 2, makecol(255, 255, 255), -1);

    load_level(level + 1);
}
/**
blit
– Stands for “BLock Transfer”
– Most important graphics function
– Takes a rectangular block of one bitmap and draws it onto
another
**/
