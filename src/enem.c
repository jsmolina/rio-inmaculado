#include "enem.h"
#include "allegro/gfx.h"
#include "allegro/platform/astdint.h"
#include "allegro/text.h"
#include "game.h"
#include "helpers.h"
#include <stdio.h>

enemyData enemies[MAX_ENEMIES];
int hitted_this_loop = FALSE;
int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];
int attack_variant = 0;

int has_alive_enemies() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (alive_enemies[level][i] == TRUE) {
            return 1;
        }
    }
    return 0;
}

void eies_sprite(enemyData *enem, unsigned int variant) {
    char file_buffer[14];
    // load enemy1
    for (int i = 0; i < 9; i++) {
        sprintf(file_buffer, "ENEM%d_%d.PCX", variant, i + 1); 
        enem->sprite[i] = load_pcx(file_buffer, NULL);
        enem->variant = variant;
        if (!enem->sprite[i]) {
            die("Cannot load %s", file_buffer);
        }
    }
}

void unload_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        for (int j = 0; j < 9; j++) {
            if (enemies[i].sprite[j]) {
                destroy_bitmap(enemies[i].sprite[j]);
            }
        }
    }
}

void init_level_enemies(int maxX, char first_load) {
    for (int ec = 0; ec < MAX_ENEMIES; ec++) {
        if (ec < levels[level].total_enemies) {
            enemies[ec].is_active = TRUE;
            enemies[ec].x = maxX - 60 - ec * 15; // it should vary per level
            enemies[ec].y = 150 + ((ec %2) * 3); // it should vary per enemy
            enemies[ec].targetX = 0;
            enemies[ec].targetY = 0;
            enemies[ec].curr_sprite = 0;
            enemies[ec].is_hit = FALSE;
            if (alive_enemies[level][ec] == TRUE) {
                enemies[ec].is_floor = FALSE;
            } else {
                enemies[ec].is_floor = HIT_DURATION;
            }
            enemies[ec].is_punching = FALSE;
            enemies[ec].received_hits = 0;
            enemies[ec].is_active = FALSE;
            
        } else {
            enemies[ec].is_active = FALSE;
            if (first_load == TRUE) {
                eies_sprite(&enemies[ec], ec % 3 + 1);
            }
            alive_enemies[level][ec] = FALSE;
        }
    }
}

void enemy_animation(enemyData *enem) {

    if (enem->is_floor != FALSE) {
        return;
    }

    if (enem->is_hit > 0) {
        enem->curr_sprite = ANIM_HITTED;
        enem->is_hit--;
        return;
    }
    if (enem->is_punching > 0) {
        enem->is_punching--;
    }

    if (enem->moving == MOVING_RIGHT || enem->moving == MOVING_LEFT) {
        if (enem->curr_sprite == ANIM_WALK1) {
            enem->curr_sprite = ANIM_WALK2;
        } else {
            enem->curr_sprite = ANIM_WALK1;
        }
    } else if (enem->moving == PUNCH_RIGHT || enem->moving == PUNCH_LEFT) {
        if (enem->curr_sprite == ANIM_PUNCH) {
            enem->curr_sprite = ANIM_PUNCH2;
        } else {
            enem->curr_sprite = ANIM_PUNCH;
        }
    } else {
        enem->curr_sprite = 0;
    }
}

int enemy_decision(enemyData *enem, spritePos *playr) {
    int distance;
    int x_distance;
    int y_distance;
    // TODO return;


    // no decisions in floor, sir
    if (enem->is_floor != FALSE) {
        return FALSE;
    }
    // do not take decisions: you are hitted
    if (enem->is_hit > 0) {
        hitted_this_loop = TRUE;
        return FALSE;
    }

    x_distance = point_distance(playr->x, enem->x);
    y_distance = point_distance(playr->y, enem->y);
    // check hits
    if (x_distance <= 24 && y_distance <= 2) {
        if (playr->moving == PUNCH_LEFT && enem->x <= playr->x && !hitted_this_loop) {
            enem->is_hit = HIT_DURATION;
            enem->received_hits++;
            hitted_this_loop = TRUE;
        }
        if (playr->moving == PUNCH_RIGHT && player.x <= enem->x && !hitted_this_loop) {
            enem->is_hit = HIT_DURATION;
            enem->received_hits++;
            hitted_this_loop = TRUE;
        }

        if (enem->received_hits == 6) {
            enem->is_floor = FLOOR_DURATION;
            enem->moving = MOVING_RIGHT;
            enem->floor_times++;
            enem->received_hits = 0;
            return TRUE;
        }
    }

    if (enem->is_punching > 0) {
        if (x_distance > 10) {
            enem->is_punching = FALSE;
        }
        return FALSE;
    }

    int random_choice = rand() % 50;
    char enem_has_moved = FALSE;
    // TODO: enemy should not tresspass hero

    if (point_distance(playr->x, enem->targetX) != FIGHT_DISTANCE || enem->targetX == FALSE) {
        if (random_choice == (8 + enem->variant)) {

            switch (enem->variant) {
                case ALEX:
                    if (playr->x < 320) {
                        enem->targetX = playr->x + FIGHT_DISTANCE - 10;
                    } else {
                        enem->targetX = playr->x - FIGHT_DISTANCE + 10;
                    }
                break;
                case JOHNY:
                    if (playr->x < 320) {
                        enem->targetX = playr->x + FIGHT_DISTANCE;
                    } else {
                        enem->targetX = playr->x - FIGHT_DISTANCE;
                    }
                    break;
                case PETER:
                    if (playr->x < 25) {
                        enem->targetX = playr->x + FIGHT_DISTANCE;
                    } else {
                        enem->targetX = playr->x - FIGHT_DISTANCE;
                    }
                    break;                
            }
        }
    } 
    if (enem->targetX != FALSE && enem->targetX != enem->x) {
        if (enem->x > enem->targetX && enem->x > 0) {
            enem->x--;
            enem_has_moved = TRUE;
            enem->moving = MOVING_LEFT;
        } else if (enem->x < enem->targetX && enem->x < 320) {
            enem->x++;
            enem_has_moved = TRUE;
            enem->moving = MOVING_RIGHT;
        } else {
            enem->moving = STOP_RIGHT;
        }
    } else {
        if (point_distance(playr->y, enem->y) >= 2 && (counter % 2) == 0) {
            if (enem->y > playr->y) {
                enem->y_moving = MOVING_UP;
                enem->y--;
            } else {
                enem->y_moving = MOVING_DOWN;
                enem->y++;
            }
            enem_has_moved = TRUE;
        } else {
            enem->y_moving = STOPPOS;

            if (enem->moving == MOVING_LEFT || enem->moving == PUNCH_LEFT) {
                enem->moving = STOP_LEFT;
                enem->targetX = FALSE;
            } else if (enem->moving == MOVING_RIGHT ||
                       enem->moving == PUNCH_RIGHT) {
                enem->moving = STOP_RIGHT;
                enem->targetX = FALSE;
            }
            if (counter % 30 == 0 && random_choice > 20 && playr->is_floor == FALSE && (point_distance(playr->x, enem->x) <= FIGHT_DISTANCE)) {
                if (enem->moving == STOP_LEFT || enem->moving == STOP_RIGHT) {
                    // TODO think on punch
                    if (enem->x > playr->x) {
                        enem->moving = PUNCH_LEFT;
                    } else {
                        enem->moving = PUNCH_RIGHT;
                    }
                    enem->is_punching = HIT_DURATION;
                }

                if ((enem->moving == PUNCH_LEFT && player.x <= enem->x && x_distance <= FIGHT_DISTANCE) 
                || (enem->moving == PUNCH_RIGHT && player.x >= enem->x && x_distance <= FIGHT_DISTANCE))  {
                    playr->is_hit = HIT_DURATION;
                    playr->received_hits++;
                    playr->lifebar--;
                    draw_lifebar();
                }
            }
        }

    }
    
    /*if (enem->variant == PETER) {
               
                sprintf(buff, "x: %d, tx: %d, enX: %d, %d", playr->x, enem->targetX, enem->x, random_choice);
                textout_ex(screen, font, "                                             ", 0,
                      50, makecol(0, 0, 0), makecol(255, 255, 255));
                textout_ex(screen, font, buff, 0,
                      50, makecol(0, 0, 0), makecol(255, 255, 255));
    } else {
        sprintf(buff, "x: %d, tx: %d, enX: %d", playr->x, enem->targetX, enem->x);
                textout_ex(screen, font, "                                             ", 0,
                      60, makecol(0, 0, 0), makecol(255, 255, 255));
                textout_ex(screen, font, buff, 0,
                      60, makecol(0, 0, 0), makecol(255, 255, 255));
    }*/
    
/*
    // check movements
    if ((enem->variant == JOHNY && (enem->targetX < playr->x || enem->targetX > playr->x + 25)) 
    || (enem->variant == PETER && (enem->targetX > playr->x || enem->targetX < (playr->x - 25)))) {    
        if ((counter % 30) == 0) {
            if (enem->variant == JOHNY) {
                enem->targetX = playr->x + 25;
            } else if (enem->variant == PETER) {                
                if (playr->x > 25) {
                    enem->targetX = playr->x - 25;
                } else {
                    enem->targetX = playr->x;
                }
            }   
        }
    } else if ((enem->variant == JOHNY && (enem->x < playr->x || enem->x > playr->x + 25)) 
    || (enem->variant == PETER && (enem->x > playr->x || enem->x < playr->x - 25))) {
        if (enem->targetX) {
            if (enem->x > enem->targetX && enem->x > 0) {
                enem->x--;
                enem_has_moved = TRUE;
                enem->moving = MOVING_LEFT;
            } else if (enem->x < enem->targetX && enem->x < 320) {
                enem->x++;
                enem_has_moved = TRUE;
                enem->moving = MOVING_RIGHT;
            } else {
                enem->moving = STOP_RIGHT;
            }
        } 
    } else {
        if (point_distance(playr->y, enem->y) >= 2 && (counter % 2) == 0) {
            if (enem->y > playr->y) {
                enem->y_moving = MOVING_UP;
                enem->y--;
            } else {
                enem->y_moving = MOVING_DOWN;
                enem->y++;
            }
            enem_has_moved = TRUE;
        } else {
            enem->y_moving = STOPPOS;

            if (enem->moving == MOVING_LEFT || enem->moving == PUNCH_LEFT) {
                enem->moving = STOP_LEFT;
                enem->targetX = FALSE;
            } else if (enem->moving == MOVING_RIGHT ||
                       enem->moving == PUNCH_RIGHT) {
                enem->moving = STOP_RIGHT;
                enem->targetX = FALSE;
            }
            if (counter % 20 == 0 && playr->is_floor == FALSE) {
                if (enem->moving == STOP_LEFT || enem->moving == STOP_RIGHT) {
                    // TODO think on punch
                    if (enem->x > playr->x) {
                        enem->moving = PUNCH_LEFT;
                    } else {
                        enem->moving = PUNCH_RIGHT;
                    }
                    enem->is_punching = HIT_DURATION;
                }

                if ((enem->moving == PUNCH_LEFT && player.x <= enem->x) || (enem->moving == PUNCH_RIGHT && player.x >= enem->x))  {
                    playr->is_hit = HIT_DURATION;
                    playr->received_hits++;
                    playr->lifebar--;                    
                    draw_lifebar();
                }
            }
        }
    }*/
    if (!enem_has_moved && (enem->moving == MOVING_LEFT || enem->moving == MOVING_RIGHT)) {
        enem->moving = STOP_LEFT;
    }
    return FALSE;
}

inline void draw_enemy(enemyData *enem) {

    if(level == 4 && urinated != FALSE) {
        // draw teacher
        draw_sprite(screen, girl, 42, 145);
    } 

    if (enem->is_floor != FALSE) {
        if (enem->moving & 1) {
            rotate_sprite(screen, enem->sprite[enem->curr_sprite], enem->x,
                            enem->y + 20, itofix(1 * 64));
        } else {
            rotate_sprite_v_flip(screen, enem->sprite[enem->curr_sprite], enem->x,
                            enem->y + 20, itofix(1 * 64));
        }

    } else {
        // redraw pair or impair?
        if (enem->moving & 1) {
            draw_sprite_h_flip(screen, enem->sprite[enem->curr_sprite], enem->x,
                            enem->y);
        } else {
            draw_sprite(screen, enem->sprite[enem->curr_sprite], enem->x, enem->y);
        }
    }

}

void all_enemy_animations() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        enemy_animation(&enemies[i]);
    }
}

void all_enemy_decisions(spritePos *playr) {
    hitted_this_loop = FALSE;
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (enemy_decision(&enemies[i], playr) == TRUE) {
            alive_enemies[level][i] = FALSE;
        }
    }
}
int enemies_y_comp(const void *a, const void *b) {
    enemyData *enemyA = (enemyData *)a;
    enemyData *enemyB = (enemyData *)b;
    return (enemyA->y - enemyB->y);
}

void all_draw_enemies() {
    qsort(enemies, levels[level].total_enemies, sizeof(enemyData), enemies_y_comp);

    for (int i = 0; i < levels[level].total_enemies; i++) {
        draw_enemy(&enemies[i]);
    }
}

void redraw_bg_enemy_positions() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        blit(bg, screen, enemies[i].x, 120, enemies[i].x, 120, 40, 80);
    }
}

int player_over_all_enemies() {
    char player_under_enemies = FALSE;
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (player.y < enemies[i].y) {
            player_under_enemies = TRUE;
        }
    }
    return player_under_enemies;
}

int enemy_on_path(unsigned int new_player_x) {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (enemies[i].is_floor != FALSE) {
            continue;
        }
        int x_distance = point_distance(new_player_x, enemies[i].x);        
        int y_distance = point_distance(player.y, enemies[i].y);

        if (y_distance < 2 && x_distance < 8) {
            return TRUE;
        }
    }
    return FALSE;
}