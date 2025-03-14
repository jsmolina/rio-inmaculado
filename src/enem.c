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

    if (enem->moving == MOVING_RIGHT || enem->moving == MOVING_LEFT || enem->y_moving == MOVING_UP|| enem->y_moving == MOVING_DOWN) {
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
        if (player.moving == PUNCH_LEFT && enem->x <= player.x && !hitted_this_loop && counter % 10 == 0) {
            play_sample(punch, 200, 80, 1200 + counter % 100, 0);  
            score += 10;
            enem->is_hit = HIT_DURATION_ENEM;
            ++enem->received_hits;
            hitted_this_loop = TRUE;
        }
        if (player.moving == PUNCH_RIGHT && player.x <= enem->x && !hitted_this_loop && counter % 10 == 0) {
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
            if (counter % 30 == 0 && random_choice > 25 && player.is_floor == FALSE && (point_distance(player.x, enem->x) <= FIGHT_DISTANCE)) {
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
                    enem->is_punching = 0;
                    player.received_hits++;
                    if (player.lifebar > 0) {
                        player.lifebar--;
                    }
                    
                    draw_lifebar();
                }
            }
        }

    }

    if (!enem_has_moved && (enem->moving == MOVING_LEFT || enem->moving == MOVING_RIGHT)) {
        enem->moving = STOP_LEFT;
    }
    return FALSE;
}

void draw_enemy(int index) {
    if ((index + 1) > levels[level].total_enemies) {
        return; 
    }

    if (enemies[index].is_floor != FALSE) {
        if (enemies[index].moving & 1) {
            draw_sprite(screen, enemies[index].sprite[11], enemies[index].x, enemies[index].y + 30);
        } else {
            draw_sprite_h_flip(screen, enemies[index].sprite[11], enemies[index].x, enemies[index].y + 30);
        }

    } else {
        if (!enemies[index].sprite[enemies[index].curr_sprite]) {
            return;
        }
        // redraw pair or impair?
        if (enemies[index].moving & 1) {
            draw_sprite_h_flip(screen, enemies[index].sprite[enemies[index].curr_sprite], enemies[index].x, enemies[index].y);
        } else {
            draw_sprite(screen, enemies[index].sprite[enemies[index].curr_sprite], enemies[index].x, enemies[index].y);
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

void enem_resets() {
    for (int i = 0; i < levels[level].total_enemies; i++) {
        enemies[i].is_punching = 0;
        enemies[i].curr_sprite = 0;
        enemies[i].targetX = 0;
        enemies[i].targetY = 0;
        if (player.x > enemies[i].x) {
            enemies[i].moving = STOP_RIGHT;
        } else {
            enemies[i].moving = STOP_LEFT;
        }
    }
}

void vespino_hitted() {
    vespino_enemy.hit = 30;
    --vespino_enemy.lifebar;
    play_sample(metalhit, 100, 80, 1200, 0);
    draw_lifebar_vespino_enemy();
}

void all_enemy_decisions() {
    hitted_this_loop = FALSE;
    for (int i = 0; i < levels[level].total_enemies; i++) {
        if (enemy_decision(&enemies[i]) == TRUE) {
            alive_enemies[level][i] = FALSE;
        }
    }
    if (level != 11) {
        return;
    }

    if (vespino_enemy.direction == VESPINO_HIDDEN && (counter % 100) == 0) {
        vespino_enemy.y = player.y - 5;
        if (vespino_enemy.x > 210) {
            vespino_enemy.direction = VESPINO_LEFT;
        } 
        if (vespino_enemy.x < 15) {
            vespino_enemy.direction = VESPINO_RIGHT;
        }
        play_sample(motorbike, 30, 80, 1200, 1); 
    } else {
        int x_distance = point_distance(vespino_enemy.x, player.x);        
        int y_distance = point_distance(vespino_enemy.y, player.y);

        if (player.is_floor == FALSE) {
                    
            if (vespino_enemy.hit > 0) {
                --vespino_enemy.hit;
                return;
            }

            if (x_distance < 20 && x_distance >= 8) { 
                if (player.moving == PUNCH_LEFT && player.x > vespino_enemy.x) {
                    vespino_hitted();
                } else  if (player.moving == PUNCH_RIGHT && player.x < vespino_enemy.x) {
                    vespino_hitted();
                    if (vespino_enemy.lifebar == 0) {
                        player.win = TRUE;
                        next_level = 0;
                        return;
                    }
                }
            }
            
            if (x_distance < 6 && y_distance < 8) {
                player.is_floor = FLOOR_DURATION / 2;
                player.received_hits = MOTORBIKE_HIT;
                
                if (player.lifebar < 2) {
                    player.lifebar = 0;
                } else {
                    player.lifebar -= 2;
                }
                draw_lifebar();
            }
        }


        if (vespino_enemy.direction == VESPINO_LEFT) {
            vespino_enemy.x -= VESPINO_SPEED;
        } else if (vespino_enemy.direction == VESPINO_RIGHT) {
            vespino_enemy.x += VESPINO_SPEED;                    
        }
        // avoid overflows
        if (vespino_enemy.x < 5 || vespino_enemy.x > 290) {
            vespino_enemy.direction = VESPINO_HIDDEN;
            clean_vespino();
            stop_sample(motorbike);
        }

        
    }
      
    
}

void draw_vespino() {
    int offset;
    if ((vespino_enemy.x / 4) % 2 == 0) {
        offset = 1;            
    } else {
        offset = 0;
    }
    if (!vespino_enemy.sprite[offset]) {
        return;
    }
    if (vespino_enemy.direction == VESPINO_LEFT) {
        draw_sprite_h_flip(screen, vespino_enemy.sprite[offset], vespino_enemy.x, vespino_enemy.y);
    } else if (vespino_enemy.direction == VESPINO_RIGHT) {
        draw_sprite(screen, vespino_enemy.sprite[offset], vespino_enemy.x, vespino_enemy.y);
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