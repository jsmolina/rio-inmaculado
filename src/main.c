/*
 * Copyright (C) 2016, Michiel Sikma <michiel@sikma.org>
 * MIT License
 */

#include <allegro.h>
#include <stdio.h>
#include "allegro/alcompat.h"
#include "allegro/datafile.h"
#include "allegro/gfx.h"
#include "game.h"

/**
 * Allegro example script. Switches to graphics mode to print "hello world",
 * then waits for a keypress and exits the program.
 * Taken from <https://wiki.allegro.cc/index.php?title=Example_ExHello>.
 * http://www.glost.eclipse.co.uk/gfoot/vivace/vivace.html
 */

int main(int argc, const char **argv)
{
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
    
    for (int i = 0; i < 9; i++) {
        sprintf(file_buffer, "MIGUEL%d.PCX", i + 1);
        player[i] = load_pcx( file_buffer, NULL ); /* load the bitmap file */ 
        if(!player[i]) {
            allegro_message(file_buffer);
            exit(1); 
        }
    }    
    bg = load_pcx("bg4.pcx", NULL);
    blit(bg, screen, 0, 0, 0, 0, 320, 200);
    
    // Wait for a keypress, then finish the program.
    exit_game = 0;               /* reset flag */
    x = 100;
    y = 150;
    do {                        /* loop */
        input();                /* get input */
        process();              /* process it */
        output();               /* give output */
        vsync();
    } while (exit_game == 0);    /* until the flag is set */

    destroy_bitmap(bg);
    for (int i = 0; i < 9; i++) {
        destroy_bitmap(player[i]); 
    }    
    for (int i = 0; i < 9; i++) {
        destroy_bitmap(playeri[i]); 
    }    
  
    return 0;
}

END_OF_MAIN()
