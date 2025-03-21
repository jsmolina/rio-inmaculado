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
#include "allegro/digi.h"
#include "allegro/draw.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "allegro/keyboard.h"
#include "allegro/midi.h"
#include "allegro/system.h"
#include "game.h"
#include "helpers.h"
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
static volatile long speed_counter = 0;

void increment_speed_counter()
{
    speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);


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

void rotate_pal(int index1, int index2) {
    int j;
    int colors;

   int auxColorR;
   int auxColorG;
   int auxColorB;


   int firstIndex = index1*3;
   int lastIndex = index2*3;
   colors = index2-index1;

   // first thing first...save last index colour
 	auxColorR = palette[lastIndex].r;
  	auxColorG = palette[lastIndex].g;
  	auxColorB = palette[lastIndex].b;

   // rotate all colors
  	for(j=0; j<colors; j++){
		palette[lastIndex -(j*3)].r = palette[lastIndex -(j*3) -3].r;
     	palette[lastIndex -(j*3)].g = palette[lastIndex -(j*3) -2].g;
     	palette[lastIndex -(j*3)].b = palette[lastIndex -(j*3) -1].b;
         set_pallete(palette);
   }

   // restore last index colour on first index
	palette[firstIndex].r = auxColorR;
	palette[firstIndex].g = auxColorG;
	palette[firstIndex].b = auxColorB;
    set_palette(palette);
}

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
    set_color_conversion(COLORCONV_NONE);
    // Switch to graphics mode, 320x200.
    set_color_depth(8);

    if (set_gfx_mode(GFX_MODEX, 320, 240, 0, 0) != 0) {
        die("Cannot set graphics mode");
    }

    install_int(fps_proc, 1000);    
    // Print a single line of "hello world" on a white screen.
    //set_palette(desktop_palette);
    double_buffer = create_video_bitmap(SCREEN_W, SCREEN_H);
    bg_video = create_video_bitmap(SCREEN_W, SCREEN_H);

    //set_color_depth(desktop_color_depth());
    slow_cpu = 1;
    clear_to_color(screen, 0);
    BITMAP *msdos = load_pcx("msdos.pcx", palette);
    if (msdos) {
        set_pallete(palette);
        blit(msdos, screen, 0, 0, 0, 0, 320, 240);    
        destroy_bitmap(msdos);
    }
    
    textout_centre_ex(screen, font, "Loading Instituto Rio Immaculado...", SCREEN_W / 2, 30, 200, -1);    
    
    extract_data(); // todo mover despues de textout

    load_levels();

    music = load_midi("ROGERR.MID");
    final_music = load_midi("WIN.MID");
    alleytheme = load_wav("alleytheme.wav");
    hit = load_wav("hit.wav");
    punch = load_wav("punch.wav");
    punch2 = load_wav("punch2.wav");
    voice = load_wav("voice.wav");
    dog_theme = load_wav("dog.wav");
    fall = load_wav("fall.wav");
    die_sample = load_wav("die.wav");
    motorbike = load_wav("moto.wav");
    metalhit = load_wav("metal.wav");

    if (!final_music || !music || !alleytheme || !hit || !punch || !punch2 || !dog_theme || !fall || !die_sample) {
        die("cannot load samples");
    }

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
        die("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }
    set_volume(230, 150);

    srand(time(NULL));
    // load tilemap
    load_tiles();
    palette[0].r = 10;
    palette[0].g = 10;
    palette[0].b = 10;
    set_pallete(palette);

    for (int i = 0; i < 12; i++) {
        sprintf(file_buffer, "MAIN%d.PCX", i + 1);
        player.sprite[i] = load_pcx( file_buffer, NULL );
        if(!player.sprite[i]) {
            die("Cannot load %s", file_buffer);
        }
    }
    player.sprite[12] = load_pcx("maind.pcx", NULL);
    if (!player.sprite[12]) {
        die("cannot load die sprite from player");
    }
    player_head = load_pcx( "HEAD.PCX", NULL );
    player_lifebar = load_pcx("LIFEBAR.PCX", NULL);
    girl = load_pcx("GIRL.PCX", NULL);
    key_sprite = load_pcx( "KEY.PCX", NULL );
    key_sprite_blue = load_pcx( "BLUE_KEY.PCX", NULL );
    vespino = load_pcx("vespino.pcx", NULL);

    if (!player_head) {
        die("cannot load head");
    }
    if (!player_lifebar) {
        die("cannot load player_lifebar");
    }
    if (!girl) {
        die("cannot load girl");
    }
    if (!key_sprite) {
        die("cannot load key_sprite");
    }
    if (!key_sprite_blue) {
        die("cannot load key_sprite_blue");
    }
    if (!vespino) {
        die("cannot load vespino");
    }

    // pre load enemies sprites
    init_enemies();
    // will load menu
    next_level = 0;
    load_level();
    

    exit_game = 0;               /* reset flag */
    player.x = 16;
    player.y = 130;
    player.lifebar = LIFEBAR;
    player.moving = STOP_RIGHT;
    player.y_moving = 0;
    player.curr_sprite = 0;
    player.is_hit = FALSE;
    player.is_floor = FALSE;
    player.received_hits = 0;
    player.lives = 3;
    player.floor_times = 0;
    cheat_mode = 0;

    gfx_init_timer();

    do {

        /*while (0 == update_count) {
            rest(0);
        }*/
        update_count = 0;
        frame_count++;
        
        if (level == MENU) {
            if (key[KEY_J] && key[KEY_S]) {
                play_sample(voice, 255, 127, 1000, 0); 
                cheat_mode = 1;
                rest(100);
                while(key[KEY_S]) {
                    rest(10);
                }
            }
            if (key[KEY_SPACE]) {
                increase_level_and_load();
                if (play_looped_midi(music, 0, -1) != 0) {
                    die("Cant play music");
                }
            }
        } else if (level == GAME_OVER) {
            level = 0;
            next_level = 0;
            load_level();
        } else {
            if (starting_level_counter == FALSE) {
                input();   /* get input */
                clean();
                process(); /* process it */
            } else {
                if ((counter & 1) == 0) {
                    clean();
                    player.y++;
                    if (counter % 10 == 0) {
                        player.curr_sprite ^= 1; // varies last digit 0/1, 1/0
                    }
                    starting_level_counter--;
                }
            }
            if (level != MENU) {
                output();  /* give output */
            }
        }
        vsync();

        if (key[KEY_ESC] && level != MISIFU_ALLEY && level != MISIFU_CHEESE) {
            exit_game = 1;
        }

    } while (exit_game == 0); /* until the flag is set */

    destroy_bitmap(bg);
    for (int i = 0; i < 12; i++) {
        destroy_bitmap(player.sprite[i]); 
    }
    destroy_bitmap(player_head);
    destroy_bitmap(player_lifebar);
    destroy_bitmap(girl);
    destroy_bitmap(key_sprite);
    destroy_bitmap(key_sprite_blue);
    destroy_bitmap(vespino);
    destroy_sample(alleytheme);
    destroy_sample(hit);
    destroy_sample(punch);
    destroy_sample(punch2);
    destroy_sample(fall);    
    destroy_sample(die_sample);
    destroy_sample(motorbike);
    destroy_sample(metalhit);
    destroy_bitmap(double_buffer);
    unload_enemies();
    destroy_tiles();
    cleanup_data();
    destroy_midi(music);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  
    return 0;
}

END_OF_MAIN()
