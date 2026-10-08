#include <allegro.h>
#include "dat_manager.h"
#include "statics.h"
#include "game.h"
#include "helpers.h"
#include "tiles.h"
#include <stdio.h>
#include <time.h>

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
    printf("Loading...");

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
    BITMAP *msdos = load_bmp("msdos.bmp", palette); // shown before datos.dat is loaded
    if (msdos) {
        set_pallete(palette);
        blit(msdos, screen, 0, 0, 0, 0, 320, 240);    
        destroy_bitmap(msdos);
    }
    textout_centre_ex(screen, font, "Loading Instituto Rio Immaculado...",
                      SCREEN_W / 2, 30, 200, -1);
    

    int saved_stdout = dup(fileno(stdout));
    freopen("/dev/null", "w", stdout);
    extract_data(); 
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);
    
    char *data_levels = dat_file[LEVELS_CSV].dat;
    load_levels(data_levels);

    music = dat_file[ROGERR_MID].dat;        // load_midi("ROGERR.MID");
    final_music = dat_file[WIN_MID].dat;     // load_midi("WIN.MID");
    alleytheme = dat_file[ALLEYTHEME_WAV].dat; // load_wav("alleytheme.wav");
    hit = dat_file[HIT_WAV].dat;
    punch = dat_file[PUNCH_WAV].dat;
    punch2 = dat_file[PUNCH2_WAV].dat;
    voice = dat_file[VOICE_WAV].dat;
    dog_theme = dat_file[DOG_WAV].dat;
    fall = dat_file[FALL_WAV].dat;
    die_sample = dat_file[DIE_WAV].dat;
    enemy_kill = dat_file[ENEMY_KILL_WAV].dat;
    motorbike = dat_file[MOTO_WAV].dat;
    metalhit = dat_file[METAL_WAV].dat;

    if (!final_music || !music || !alleytheme || !hit || !punch || !punch2 || !dog_theme || !fall || !die_sample || !enemy_kill) {
        die("cannot load samples");
    }

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "./allegro.cfg") != 0) {
        die("Error: inicializando sistema de sonido\n%s\n", allegro_error);
    }

    srand(time(NULL));
    // load tilemap
    load_tiles(dat_file);
    palette[0].r = 10;
    palette[0].g = 10;
    palette[0].b = 10;
    set_pallete(palette);

    for (int i = 0; i < 12; i++) {
        player.sprite[i] = dat_frame(PLAYER_BMP, 40 * i, 40, 40);
        if(!player.sprite[i]) {
            die("Cannot load player frame %d", i);
        }
    }
    player.sprite[12] = dat_file[MAIND_BMP].dat;
    if (!player.sprite[12]) {
        die("cannot load die sprite from player");
    }
    player_head = dat_file[HEAD_BMP].dat;
    player_lifebar = dat_file[LIFEBAR_BMP].dat;
    girl = dat_file[GIRL_BMP].dat;
    key_sprite = dat_file[KEY_BMP].dat;
    key_sprite_blue =  dat_file[BLUE_KEY_BMP].dat;
    vespino = dat_file[VESPINO_BMP].dat;

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
    init_enemies(dat_file);
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
    player.jump = 0;
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
                if (play_looped_midi(music, 0, -1) != 0) {
                    die("Cant play music");
                }
                increase_level_and_load();                
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

    /*destroy_bitmap(bg);
    for (int i = 0; i < 12; i++) {
        destroy_bitmap(player.sprite[i]); 
    }*/
    unload_datafile(dat_file);
    //unload_enemies();
    //destroy_tiles();
    //cleanup_data();
    //destroy_midi(music);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  
    return 0;
}

END_OF_MAIN()
