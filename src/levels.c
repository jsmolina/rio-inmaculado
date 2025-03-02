#include <allegro.h>
#include "allegro/gfx.h"
#include "allegro/keyboard.h"
#include "allegro/text.h"
#include <stdio.h>
#include "game.h"
#include "helpers.h"
#include "tiles.h"
#include "levels.h"


int velocidad = VELOCIDAD_BASE;
char sentido = DERECHA;
char beep_count = -1;
unsigned char missed_beeps = 0;
char beep_side = IZQUIERDA;
unsigned char ultima_tecla = 0;
clock_t ultimo_tiempo = 0;
clock_t ultimo_beep = 0;



unsigned char automatic_event = FALSE;


// returns true if player is >= or <= a margin on a door
inline int is_on_door(int door_x) {
    if (door_x == 0) {
        return FALSE;
    }
    int res = (player.x >= door_x && player.x <= (door_x + 50));
    if (res && has_alive_enemies()) {
        textout_ex(screen, font, "Cerrado", 120, SCREEN_H - 34, makecol(100,255,255), getpixel(screen, 319, 239));               
        rest(500);
        textout_ex(screen, font, "       ", 120, SCREEN_H - 34, makecol(100,255,255), getpixel(screen, 319, 239));               
        return FALSE;
    }
    return res;
}

inline int move_to_level_if_needed() {
    if (level == 0) {
        return FALSE;
    }
    LevelData curr_leveldata = levels[level];
    if (player.y < 142) {
        if (key[KEY_SPACE]) {
            // for doors stays on same level but with subdoors
            if (is_on_door(curr_leveldata.door1Pos) && (level != 10 || blue_key == TRUE)) {
                next_level = curr_leveldata.door1;
                
                return TRUE;
            } else if (is_on_door(curr_leveldata.door2Pos) && (level != 7 || yellow_key == TRUE)) {
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
    } else if (curr_leveldata.left != 0 && player.x <= (curr_leveldata.minX)) {
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

void level8_coursnave() {
    if (player.x < 39) {
        player.x += 1;
        player.moving = MOVING_RIGHT;
        return;
    }
    textout_ex(screen, font, "COURSNAVE. ", 120, SCREEN_H - 34, makecol(255, 255, 255), -1);    

    if (beep_count < 0) {
        textout_ex(screen, font, "PRESS SPACE TO START (KEYS Z/X). ", 40, SCREEN_H - 10, makecol(255, 255, 255), makecol(0,0,0));
        while (!key[KEY_SPACE] && !key[KEY_ESC]) {
            rest(1);
        }
        beep_count = 0;
        beep(2000, 10);
    } else {
        textout_ex(screen, font, "GO GO GO GO          (KEYS Z/X). ", 40, SCREEN_H - 10, makecol(200, 200, 255), makecol(0,0,0));
    }
    clock_t tiempo_actual = clock();


    char missed[16];
    sprintf(missed, "FAIL: %01d", missed_beeps);
    textout_ex(screen, font, missed, 120, SCREEN_H - 26, makecol(255, 0, 0), makecol(0, 0, 0));
    sprintf(missed, "COURSE: %01d", beep_count);
    textout_ex(screen, font, missed, 210, SCREEN_H - 26, makecol(0, 100, 255), makecol(0, 0, 0));

    float tiempo_entre_beeps = (float)(tiempo_actual - ultimo_beep) / CLOCKS_PER_SEC;
    if (ultimo_beep == 0 || tiempo_entre_beeps > (4 - beep_count * 0.25)) {
        ultimo_beep = clock();
        if (beep_side == IZQUIERDA) {
            beep(2000, 50);
            beep_side = DERECHA;
            if (player.x > 70) {
                missed_beeps += 1;
            } else {
                beep_count+=1;
            }
        } else {
            beep(1500, 60);
            beep_side = IZQUIERDA;            
            if (player.x < 200) {
                missed_beeps += 1;
            } else {
                beep_count+=1;
            }
        }
    }
    if (beep_count == 8) {
        coursnave_completed = TRUE;
        textout_ex(screen, font, "COMPLETADO!", 180, 80, makecol(0, 255, 0), makecol(0, 0, 0));
        beep(2000, 20);
        beep(2000, 20);
        rest(1000);
        next_level = 7;
        return;
    } else if (missed_beeps == 4) {
        textout_ex(screen, font, "REPITES!", 180, 80, makecol(255,79, 0), makecol(0, 0, 0));
        beep(800, 100);
        beep(800, 100);
        next_level = 7;
        beep_count = -1;
        missed_beeps = 0;
        beep_side = IZQUIERDA;
        return;
    }

    float tiempo_transcurrido = (float)(tiempo_actual - ultimo_tiempo) / CLOCKS_PER_SEC;
    char buf[25];
    if (key[KEY_Z] && ultima_tecla != KEY_Z) {
        //velocidad += (1.0 / tiempo_transcurrido);
        ultima_tecla = KEY_Z;
        ultimo_tiempo = tiempo_actual;
    } else if (key[KEY_X] && ultima_tecla != KEY_X) {
        if (tiempo_transcurrido > 1) {
            velocidad = 0;
        } else if (tiempo_transcurrido >= 0.5) {
            velocidad = 1;
        } else if (tiempo_transcurrido >= 0.4) {
            velocidad = 2;
        } else if (tiempo_transcurrido >= 0.3) {
            velocidad = 3;
        } else if (tiempo_transcurrido >= 0.1) {
            velocidad = 4;
        }
        ultima_tecla = KEY_X;
        ultimo_tiempo = tiempo_actual;
    } 

    if (tiempo_transcurrido > 0.1) {
        velocidad -= 1;
        if (velocidad  < 1) {
            velocidad = 1;
        }
    }

    // cambio de sentido si llega al tope (signo +1 o -1)
    if (player.x > 230 && beep_side == DERECHA) {

    } else if (player.x < 40 && beep_side == IZQUIERDA) {

    } else {
        player.x += (velocidad * sentido);
    }

    if (player.x > 230) {
        sentido = IZQUIERDA;
    } else if (player.x < 40) {
        sentido = DERECHA;
    }


    if (velocidad > 0) {
        if (sentido == DERECHA) {
            player.moving = MOVING_RIGHT;
        } else {
            player.moving = MOVING_LEFT;
        }
    } else if(tiempo_transcurrido > 0.2) {
        if (sentido == DERECHA) {
            player.moving = STOP_RIGHT;
        } else {
            player.moving = STOP_LEFT;
        }
    }
}

void move_with_level_limits() {
    LevelData curr_leveldata = levels[level];
    unsigned int minY, maxY;
    unsigned int minX, maxX;

    minX = curr_leveldata.minX;
    maxX = curr_leveldata.maxX;
    
    switch (level) {
        case 9:
            minX = 0;
            minY = 69;
            maxY = 151;
            maxX = 15 + (151 - player.y);
            if (player.x > maxX) {
                maxY = player.y;
            }
        break;
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
        case 7:
            if (player.x < 213 && player.x > 40) {
                minY = 145;
                maxY = 151;
                if (player.y < minY) {
                    if (player.x > 200) minX = 213;
                    else if (player.x > 40) maxX = 40;
                }
            } else if (player.x > 242) {
                minY = 145;
                maxY = 151;
                if (player.y < minY) {
                    maxX = 242;
                }
            } else {
                minY = 140;
                maxY = 151;
            }
            break;
        case 10:
            if (player.x < 252 && player.x > 27) {
                minY = 145;
                maxY = 151;
                if (player.y < minY) {
                    if (player.x > 242) {
                        minX = 252;
                    } else if (player.x < 37) {
                        maxX = 27;
                    }                    
                }
            } else {
                minY = 140;
                maxY = 151;
            }
            break;
        case 11:
            minY = 120;
            maxY = 151;
        break;
        default:
            minY = 140;
            maxY = 151;            
    }
    if (level == 8 && !coursnave_completed) {
        level8_coursnave();
        return;
    } else {
        // normal movement
        if (player.moving == MOVING_RIGHT && player.x < maxX) {
            if (!enemy_on_path(player.x + 1)) {
                    ++player.x;
            }
        } else if (player.moving == MOVING_LEFT && player.x > minX) {
            if (!enemy_on_path(player.x - 1)) {
                --player.x;
            }
        }

        if (player.y_moving == MOVING_DOWN && player.y < maxY) {
            player.y++;
        } 
        
        if (player.y_moving == MOVING_UP && player.y > minY) {
            player.y--;
        }
    }
}

void loop_castigo() {
    int index = 0;
    int key2;
    
    textout_ex(screen, font, "no entrare al de las chicas", 21, 35, makecol(0,0,0), -1);

    char compar[] = "no entrare al de las chicas";
    char buf[] =    "                            ";
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
        
        textout_ex(screen, font, buf, 21, 51, makecol(0,0,0), -1);
        rest(1); 
    }
    //castigo = CASTIGO_FINALIZADO;
    next_level = 3;
    return;
}

void level_processing() {

    switch (level) {
        case 2:
            if (key[KEY_SPACE]) {
                //76, 84, 92
                if (player.y < 142 && player.x >= 204 && player.x <= 219) {
                    textout_ex(screen, font, "bedel?        ", 120, SCREEN_H - 34, makecol(100,255,255), makecol(0,0,0));               
                    textout_ex(screen, font, "quien eres tu?", 120, SCREEN_H - 26, makecol(255,255,255), makecol(0,0,0));
                    textout_ex(screen, font, "arregla el ascensor!", 120, SCREEN_H - 18, makecol(100,255,255), makecol(0,0,0));
                    locked_elevator = FALSE;
                }
            }
        break;
        case 4:
            if (key[KEY_SPACE]) {

                if (player.y < 147 && player.x >= 7 && player.x <= 28) {
                    player.curr_sprite = ANIM_ESPALDA;
                    player.moving = LOOKING_WALL;
                    if (urinated == FALSE) {
                        textout_ex(screen, font, "ahhh, mejor asi!", 120, SCREEN_H - 34, makecol(100,255,255), makecol(0,0,0));               
                        textout_ex(screen, font, "largate o te castigo!", 120, SCREEN_H - 26, makecol(194,106,228), makecol(0,0,0));
                        urinated = TRUE;
                        player.lifebar = LIFEBAR;
                        draw_lifebar();
                        while (key[KEY_SPACE]) {
                            rest(1); // Pequeña pausa para no saturar el CPU
                        }
                    } else {
                        textout_centre_ex(screen, font, "vas a escribir 1 veces", SCREEN_W / 2, 76, makecol(194,106,228), makecol(0,0,0));
                        textout_centre_ex(screen, font, "no entrare al de las chicas", SCREEN_W / 2, 84, makecol(194,106,228), makecol(0,0,0));
                        ///
                        rest(500);
                        while (key[KEY_SPACE]) {
                            rest(1); // Pequeña pausa para no saturar el CPU
                        }
                        next_level = 12;
                        return;
                    }

                }
            }
            
        break;
        case 5:
            
        break;
        case 12:
            loop_castigo();
        break;
        case 9:
        if (key[KEY_SPACE] && yellow_key != TRUE) {
            if (player.y < 110 && player.x >= 20 && player.x <= 45) {
                yellow_key = TRUE;
            }
        }
        break;
        case 10:
        if (key[KEY_SPACE]) {
            //76, 84, 92
            if (player.y < 147 && player.x >= 28 && player.x <= 252) {
                next_level = MISIFU_ALLEY;
                return;
            }
        }
        break;
    }
    
}