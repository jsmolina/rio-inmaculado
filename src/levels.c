#include <allegro.h>
#include "allegro/gfx.h"
#include "allegro/keyboard.h"
#include "allegro/text.h"
#include <stdio.h>
#include "game.h"
#include "tiles.h"
#include "levels.h"



unsigned char automatic_event = FALSE;


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

            if (is_on_door(curr_leveldata.elevatorPos) && !locked_elevator) {
                next_level = curr_leveldata.elevator;
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


BITMAP * load_level_background(unsigned char lvl) {
    char level_filename[17];
    switch (lvl) {
        case 5:
            if (locked_elevator) {
                sprintf(level_filename, "bg5_0.tmx");
            } else {
                sprintf(level_filename, "bg5.tmx");
            }
        break;
        default:
            sprintf(level_filename, "bg%d.tmx", lvl);        
    }

    return load_background(level_filename);
}

void move_with_level_limits() {
    LevelData curr_leveldata = levels[level];
    unsigned int minY, maxY;
    unsigned int minX, maxX;

    minX = curr_leveldata.minX;
    maxX = curr_leveldata.maxX;
    
    switch (level) {
        case 4:
            if (player.x < 263) {
                minY = 145;
                maxY = 151;
                if (player.y < minY) {
                    minX = 263;
                }
            } else {
                minY = 140;
                maxY = 151;
            }
            break;
        default:
            minY = 140;
            maxY = 151;            
    }
    if (player.moving == MOVING_RIGHT && player.x < maxX) {
        if (!enemy_on_path(player.x + 1, player.y)) {
                player.x++;
        }
    } else if (player.moving == MOVING_LEFT && player.x > minX) {
        if (!enemy_on_path(player.x - 1, player.y)) {
            player.x--;
        }
    }

    if (player.y_moving == MOVING_DOWN && player.y < maxY) {
        player.y++;
    } 
    
    if (player.y_moving == MOVING_UP && player.y > minY) {
        player.y--;
    }
}

void loop_castigo() {
    int index = 0;
    int key2;
    
    clear_keybuf(); // Limpia el buffer del teclado
    textout_ex(screen, font, "no entrare al de las chicas", 21, 35, makecol(194,106,228), -1);

    char compar[] = "no entrare al de las chicas";
    char buf[] =    "                           ";
    while (index < 27) {
        if (keypressed()) {
            key2 = readkey();
            
            if (index < 30 - 1) {
                char shifted = key2 & 0xFF;
                if (shifted == compar[index]) {
                    buf[index] = shifted;
                    index++;
                }
            }
            if (key[KEY_ESC]) {
                return;
            }
  
        }
        
        textout_ex(screen, font, buf, 21, 51, makecol(50,125,125), -1);
        rest(1); 
    }
    castigo = CASTIGO_FINALIZADO;
}

void level_processing() {

    switch (level) {
        case 2:
            if (key[KEY_SPACE]) {
                if (player.y < 142 && player.x >= 204 && player.x <= 219) {
                    textout_centre_ex(screen, font, "bedel?", SCREEN_W / 2, 76, makecol(100,255,255), makecol(0,0,0));               
                    textout_centre_ex(screen, font, "quien eres tu?", SCREEN_W / 2, 84, makecol(255,255,255), makecol(0,0,0));
                    textout_centre_ex(screen, font, "no importa, arregla el ascensor", SCREEN_W / 2, 92, makecol(100,255,255), makecol(0,0,0));
                    locked_elevator = FALSE;
                }
            }
        break;
        case 4:
            if (key[KEY_SPACE]) {
                if (player.y < 147 && player.x >= 7 && player.x <= 28) {
                    if (!urinated) {
                        textout_centre_ex(screen, font, "ahhh, mejor asi!", SCREEN_W / 2, 76, makecol(100,255,255), makecol(0,0,0));               
                        textout_centre_ex(screen, font, "largate o te castigo!", SCREEN_W / 2, 84, makecol(194,106,228), makecol(0,0,0));
                        urinated = TRUE;
                        player.lifebar = LIFEBAR;
                        draw_lifebar();
                        while (key[KEY_SPACE]) {
                            rest(1); // Pequeña pausa para no saturar el CPU
                        }
                    } else {
                        textout_centre_ex(screen, font, "vas a escribir veces", SCREEN_W / 2, 76, makecol(194,106,228), makecol(0,0,0));
                        textout_centre_ex(screen, font, "no entrare al de las chicas", SCREEN_W / 2, 84, makecol(194,106,228), makecol(0,0,0));
                        castigo = TRUE;
                    }

                }
            }
        break;
        case 5:
            
        break;
        case 12:
            loop_castigo();
        break;
    }
    
}