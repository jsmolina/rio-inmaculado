/*
 * Copyright (C) 2016, Michiel Sikma <michiel@sikma.org>
 * MIT License
 */
#include <time.h>
#include <allegro.h>
#include <stdio.h>
#include "allegro/alcompat.h"
#include "allegro/color.h"
#include "allegro/datafile.h"
#include "allegro/gfx.h"
#include "allegro/keyboard.h"
#include "allegro/system.h"
#include "game.h"
#include "tiles.h"
#include "dat_manager.h"

static volatile int update_count, frame_count, fps = 0;

void gfx_timer_proc(void) { update_count = 1; }
END_OF_FUNCTION(gfx_timer_proc)

static void gfx_fps_proc(void) {
    fps = frame_count;
    frame_count = 0;
}
END_OF_FUNCTION(gfx_fps_proc)

void gfx_init_timer() {
    LOCK_VARIABLE(update_count);
    LOCK_VARIABLE(frame_count);
    LOCK_VARIABLE(fps);
    LOCK_FUNCTION(gfx_timer_proc);
    LOCK_FUNCTION(gfx_fps_proc);
    install_int_ex(gfx_timer_proc, BPS_TO_TIMER(70));
    install_int_ex(gfx_fps_proc, BPS_TO_TIMER(1));
}

/**
 * Allegro example script. Switches to graphics mode to print "hello world",
 * then waits for a keypress and exits the program.
 * Taken from <https://wiki.allegro.cc/index.php?title=Example_ExHello>.
 * http://www.glost.eclipse.co.uk/gfoot/vivace/vivace.html
 */
/* timer callback for measuring the frames per second */
static void fps_proc(void) {
    fps = frame_count;
    frame_count = 0;
}

END_OF_STATIC_FUNCTION(fps_proc);



int main(int argc, const char **argv) {
    char file_buffer[14];
    BITMAP *bmp;
    
    // Initializes the Allegro library.
    if (allegro_init() != 0) {
        return 1;
    }

    // Installs the Allegro keyboard interrupt handler.
    install_keyboard();
    install_timer();


    //bmp = create_bitmap(640, 480);
    set_color_conversion(COLORCONV_TOTAL);
    // Switch to graphics mode, 320x200.
    set_color_depth(8);

    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Cannot set graphics mode:\r\n%s\r\n", allegro_error);
        return 1;
    }

    install_int(fps_proc, 1000);    
    // Print a single line of "hello world" on a white screen.
    //set_palette(desktop_palette);

    //set_color_depth(desktop_color_depth());
    clear_to_color(screen, 0);
    textout_centre_ex(screen, font, "Loading Instituto Rio Immaculado...", SCREEN_W / 2, SCREEN_H / 2, makecol(255,255,255), -1);
    extract_data(); // todo mover despues de textout

    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0);
    

    for (int i = 0; i < 9; i++) {
        sprintf(file_buffer, "MAIN%d.PCX", i + 1);
        player.sprite[i] = load_pcx( file_buffer, NULL );
        if(!player.sprite[i]) {
            allegro_message("cannot load: %s", file_buffer);
            exit(1); 
        }
    }

    srand(time(NULL));
    // load tilemap
    load_tiles();
    // pre load enemies sprites
    init_level_enemies(0, TRUE);
    // will load menu
    
    load_level(0);
    

    exit_game = 0;               /* reset flag */
    player.x = 100;
    player.y = 150;
    player.moving = STOP_RIGHT;
    player.y_moving = 0;
    player.curr_sprite = 0;
    player.is_hit = FALSE;
    player.is_floor = FALSE;
    player.received_hits = 0;
    player.lives = 3;
    player.floor_times = 0;
    gfx_init_timer();

    do {
        while (0 == update_count) {
            rest(0);
            if (key[KEY_ESC]) {
                exit_game = 1;
                break;
            }
            vsync();

        }
        update_count = 0;
        frame_count++;

        if (level == 0) {
            if (key[KEY_SPACE]) {
                increase_level_and_load();
            }
        } else {
            input();   /* get input */
            process(); /* process it */
            output();  /* give output */
        }

        if (key[KEY_ESC]) {
            exit_game = 1;
        }

    } while (exit_game == 0); /* until the flag is set */

    destroy_bitmap(bg);
    for (int i = 0; i < 9; i++) {
        destroy_bitmap(player.sprite[i]); 
    }
    unload_enemies();
    destroy_tiles();
    cleanup_data();
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  
    return 0;
}

END_OF_MAIN()
