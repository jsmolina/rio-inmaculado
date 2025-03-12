#include "enem.h"
#include "allegro/datafile.h"
#include "allegro/digi.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "allegro/platform/astdint.h"
#include "allegro/text.h"
#include "game.h"
#include "helpers.h"
#include <stdio.h>

enemyData enemies[MAX_ENEMIES];
vespinoData vespino_enemy;

int hitted_this_loop = FALSE;
int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];
int attack_variant = 0;

int has_alive_enemies() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (enemies[i].is_floor == FALSE) {
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
    // load dead position
    sprintf(file_buffer, "ENEM%dd.PCX", variant); 
    enem->sprite[11] = load_pcx(file_buffer, NULL);
    if (!enem->sprite[11]) {
        die("cannot load died enem%dd.pcx", enem->variant);
    }
}

void unload_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        for (int j = 0; j < 9; j++) {
            if (enemies[i].sprite[j]) {
                destroy_bitmap(enemies[i].sprite[j]);
            }
        }
        destroy_bitmap(enemies[i].sprite[11]);
    }
    for (int i = 0; i < 2; i++) {
        destroy_bitmap(vespino_enemy.sprite[i]);
    }
    
}

void init_enemies() {
    for (int ec = 0; ec < MAX_ENEMIES; ec++) {
        eies_sprite(&enemies[ec], ec % 3 + 1);
        alive_enemies[level][ec] = FALSE;
    }
    vespino_enemy.sprite[0] = load_pcx("vespino2.pcx", NULL);
    vespino_enemy.sprite[1] = load_pcx("vespino3.pcx", NULL);
}

void init_level_enemies() {
    for (int ec = 0; ec < levels[level].total_enemies; ec++) {
        enemies[ec].x = levels[level].maxX - 60 - ec * 15; // it should vary per level
        enemies[ec].y = 150 + ((ec % 2) * 3); // it should vary per enemy
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
    }
}

void enemy_animation(enemyData *enem) {

    if (enem->is_floor != FALSE) {
        enem->curr_sprite = 0;
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

int enemy_decision(enemyData *enem) {
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

    x_distance = point_distance(player.x, enem->x);
    y_distance = point_distance(player.y, enem->y);
    // check hits
    if (x_distance <= 24 && y_distance <= 2) {
        if (player.moving == PUNCH_LEFT && enem->x <= player.x && !hitted_this_loop && counter != 100) {
            play_sample(punch, 200, 80, 1200 + counter % 100, 0);  
            score += 10;
            enem->is_hit = HIT_DURATION_ENEM;
            ++enem->received_hits;
            hitted_this_loop = TRUE;
        }
        if (player.moving == PUNCH_RIGHT && player.x <= enem->x && !hitted_this_loop && counter != 100) {
            score += 10;
            play_sample(punch, 200, 155, 1200 + counter % 100, 0);  
            enem->is_hit = HIT_DURATION_ENEM;
            ++enem->received_hits;
            hitted_this_loop = TRUE;
        }

        if (enem->received_hits == 8) {
            stop_sample(punch);
            play_sample(fall, 255, 127, 1000, 0);  
            enem->is_floor = FLOOR_DURATION;
            enem->moving = MOVING_RIGHT;
            ++enem->floor_times;
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

    if (point_distance(player.x, enem->targetX) != FIGHT_DISTANCE || enem->targetX == FALSE) {
        if (random_choice == (8 + enem->variant)) {

            switch (enem->variant) {
                case ALEX:
                    if (player.x < 320) {
                        enem->targetX = player.x + FIGHT_DISTANCE - 10;
                    } else {
                        enem->targetX = player.x - FIGHT_DISTANCE + 10;
                    }
                break;
                case JOHNY:
                    if (player.x < 320) {
                        enem->targetX = player.x + FIGHT_DISTANCE;
                    } else {
                        enem->targetX = player.x - FIGHT_DISTANCE;
                    }
                    break;
                case PETER:
                    if (player.x < 25) {
                        enem->targetX = player.x + FIGHT_DISTANCE;
                    } else {
                        enem->targetX = player.x - FIGHT_DISTANCE;
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
        if (point_distance(player.y, enem->y) >= 2 && (counter % 2) == 0) {
            if (enem->y > player.y) {
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
            if (counter % 30 == 0  && player.is_floor == FALSE && (point_distance(player.x, enem->x) <= FIGHT_DISTANCE)) {
                if (enem->moving == STOP_LEFT || enem->moving == STOP_RIGHT) {
                    // TODO think on punch
                    if (enem->x > player.x) {
                        enem->moving = PUNCH_LEFT;
                    } else {
                        enem->moving = PUNCH_RIGHT;
                    }
                    enem->is_punching = HIT_DURATION;
                }

                if ((enem->moving == PUNCH_LEFT && player.x <= enem->x && x_distance <= FIGHT_DISTANCE) 
                || (enem->moving == PUNCH_RIGHT && player.x >= enem->x && x_distance <= FIGHT_DISTANCE))  {
                    player.is_hit = HIT_DURATION;
                    play_sample(punch2, 200, 127, 1200 + counter % 100, 0); 
                    player.received_hits++;
                    if (player.lifebar > 0) {
                        player.lifebar--;
                    }
                    
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
            draw_sprite(screen, enem->sprite[11], enem->x, enem->y + 30);
        } else {
            draw_sprite_h_flip(screen, enem->sprite[11], enem->x, enem->y + 30);
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
void clean_vespino() {
    blit(bg, screen, vespino_enemy.x - 3, vespino_enemy.y, vespino_enemy.x-3, vespino_enemy.y, 55, 50);
}

void all_enemy_decisions() {
    hitted_this_loop = FALSE;
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (enemy_decision(&enemies[i]) == TRUE) {
            alive_enemies[level][i] = FALSE;
        }
    }

    if (vespino_enemy.direction == VESPINO_HIDDEN && counter == 100) {
        vespino_enemy.y = player.y - 5;
        if (vespino_enemy.x > 210) {
            vespino_enemy.direction = VESPINO_LEFT;
        } 
        if (vespino_enemy.x < 15) {
            vespino_enemy.direction = VESPINO_RIGHT;
        }
    } else {
        if (vespino_enemy.direction == VESPINO_LEFT) {
            vespino_enemy.x -= VESPINO_SPEED;
        } else if (vespino_enemy.direction == VESPINO_RIGHT) {
            vespino_enemy.x += VESPINO_SPEED;                    
        }
        // avoid overflows
        if (vespino_enemy.x < 5) {
            vespino_enemy.direction = VESPINO_HIDDEN;
            clean_vespino();
        }
        if (vespino_enemy.x > 290) {
            vespino_enemy.direction = VESPINO_HIDDEN;
            clean_vespino();
        }
        if (player.is_floor == FALSE) {
            int x_distance = point_distance(vespino_enemy.x, player.x);        
            int y_distance = point_distance(vespino_enemy.y, player.y);
            if (x_distance < 8 && y_distance < 8) {
                player.is_floor = FLOOR_DURATION / 2;
                player.received_hits = MOTORBIKE_HIT;
                
                if (player.lifebar < 4) {
                    player.lifebar = 0;
                } else {
                    player.lifebar -= 4;
                }
                draw_lifebar();
            }
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

    if (level == 11) {
        int offset;
        if ((vespino_enemy.x / 4) % 2 == 0) {
            offset = 1;            
        } else {
            offset = 0;
        }
        
        if (vespino_enemy.direction == VESPINO_LEFT) {
            draw_sprite_h_flip(screen, vespino_enemy.sprite[offset], vespino_enemy.x, vespino_enemy.y);
        } else if (vespino_enemy.direction == VESPINO_RIGHT) {
            draw_sprite(screen, vespino_enemy.sprite[offset], vespino_enemy.x, vespino_enemy.y);
        }
    }
}

void redraw_bg_enemy_positions() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        blit(bg, screen, enemies[i].x, 120, enemies[i].x, 120, 40, 80);
    }
    if (vespino_enemy.direction != VESPINO_HIDDEN) {
        clean_vespino();
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