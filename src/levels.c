#include "allegro/keyboard.h"
#include "allegro/text.h"
#include <stdio.h>
#include "game.h"
#include "tiles.h"



unsigned char automatic_event = FALSE;


void level2_processing() {
    if (key[KEY_RCONTROL] || key[KEY_LCONTROL]) {          
        if (player.y < 142 && player.x >= 204 && player.x <= 219) {
            textout_centre_ex(screen, font, "support?", SCREEN_W / 2, 76, makecol(100,255,255), makecol(0,0,0));               
            textout_centre_ex(screen, font, "who are you?", SCREEN_W / 2, 84, makecol(255,255,255), makecol(0,0,0));
            textout_centre_ex(screen, font, "nevermind, please fix the elevator", SCREEN_W / 2, 92, makecol(100,255,255), makecol(0,0,0));
            locked_elevator = FALSE;
        }
    }
}

void load_level_background() {
    char level_filename[17];
    switch (next_level) {
        case 5:
            if (locked_elevator) {
                sprintf(level_filename, "bg5_0.tmx");
            } else {
                sprintf(level_filename, "bg5.tmx");
            }
        break;
        default:
            sprintf(level_filename, "bg%d.tmx", next_level);        
    }

    bg = load_background(level_filename);
}

void display_lives() {
    
}