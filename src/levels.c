#include "allegro/keyboard.h"
#include "allegro/text.h"
#include <stdio.h>
#include "game.h"
#include "tiles.h"



unsigned char automatic_event = FALSE;


void level2_processing() {
    if (key[KEY_SPACE]) {          
        if (player.y < 142 && player.x >= 204 && player.x <= 219) {
            textout_centre_ex(screen, font, "support?", SCREEN_W / 2, 76, makecol(100,255,255), makecol(0,0,0));               
            textout_centre_ex(screen, font, "who are you?", SCREEN_W / 2, 84, makecol(255,255,255), makecol(0,0,0));
            textout_centre_ex(screen, font, "nevermind, please fix the elevator", SCREEN_W / 2, 92, makecol(100,255,255), makecol(0,0,0));
            locked_elevator = FALSE;
        }
    }
}


// returns true if player is >= or <= a margin on a door
inline unsigned char is_on_door(int door_x) {
    if (door_x == 0) {
        return FALSE;
    }
    return (player.x >= door_x && player.x <= (door_x + 50));
}

inline unsigned char move_to_level_if_needed() {
    if (level == 0) {
        return FALSE;
    }
    LevelData curr_leveldata = levels[level];
    if (player.y < 142) {
        if (key[KEY_SPACE]) {
            // for doors stays on same level but with subdoors
            if (is_on_door(curr_leveldata.door1Pos)) {
                next_level = curr_leveldata.door1;
                
                return TRUE;
            } else if (is_on_door(curr_leveldata.door2Pos)) {
                next_level = curr_leveldata.door2;
                return TRUE;
            }
        }
    }
    // right and left sides
    if (curr_leveldata.right != 0 && player.x >= (curr_leveldata.maxX - 5)) {
        next_level = curr_leveldata.right;
        return TRUE;
    } else if (curr_leveldata.left != 0 && player.x <= (curr_leveldata.minX + 5)) {
        next_level = curr_leveldata.left;
        return TRUE;
    }
    return FALSE;    
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