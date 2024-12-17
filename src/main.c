/*
 * Copyright (C) 2016, Michiel Sikma <michiel@sikma.org>
 * MIT License
 */

#include <allegro.h>
#include <stdio.h>
#include "allegro/alcompat.h"
#include "allegro/datafile.h"
#include "allegro/gfx.h"
#include "allegro/keyboard.h"
#include "game.h"
#include "tiles.h"
#include "dat_manager.h"

/**
 * Allegro example script. Switches to graphics mode to print "hello world",
 * then waits for a keypress and exits the program.
 * Taken from <https://wiki.allegro.cc/index.php?title=Example_ExHello>.
 * http://www.glost.eclipse.co.uk/gfoot/vivace/vivace.html
 */

int main(int argc, const char **argv) {
    char file_buffer[14];
    BITMAP *bmp;
    
    // Initializes the Allegro library.
    if (allegro_init() != 0) {
        return 1;
    }

    // Installs the Allegro keyboard interrupt handler.
    install_keyboard();
    //bmp = create_bitmap(640, 480);
    set_color_depth(32);
    // Switch to graphics mode, 320x200.
    if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Cannot set graphics mode:\r\n%s\r\n", allegro_error);
        return 1;
    }
    // Print a single line of "hello world" on a white screen.
    //set_palette(desktop_palette);
    //set_color_depth(desktop_color_depth());
    clear_to_color(screen, TRANS);
    textout_centre_ex(screen, font, "Loading Instituto Rio Immaculado...", SCREEN_W / 2, SCREEN_H / 2, makecol(0,0,0), -1);
    extract_data(); // todo mover despues de textout

    for (int i = 0; i < 9; i++) {
        sprintf(file_buffer, "MAIN%d.PCX", i + 1);
        player.sprite[i] = load_pcx( file_buffer, NULL );
        if(!player.sprite[i]) {
            allegro_message(file_buffer);
            exit(1); 
        }
    }    
    srand(time(0));
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

    do {
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
        vsync();

    } while (exit_game == 0); /* until the flag is set */

    destroy_bitmap(bg);
    for (int i = 0; i < 9; i++) {
        destroy_bitmap(player.sprite[i]); 
    }
    unload_enemies();
    destroy_tiles();
    cleanup_data();
  
    return 0;
}

END_OF_MAIN()
