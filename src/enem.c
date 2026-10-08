#include "enem.h"
#include "allegro/datafile.h"
#include "allegro/digi.h"
#include "allegro/gfx.h"
#include "allegro/inline/draw.inl"
#include "game.h"
#include "helpers.h"
#include <stdio.h>
#include "statics.h"
#include "dat_manager.h"

enemyData enemies[MAX_ENEMIES];
vespinoData vespino_enemy;

int hitted_this_loop = FALSE;
int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];
int attack_variant = 0;
unsigned char alive_enemies_count = 0;

int enemy_in_room(int i) {
    return level < TOTAL_LEVELS && alive_enemies[level][i] != ENEMY_NONE && enemies[i].enter_delay == 0;
}

int has_alive_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (alive_enemies[level][i] == TRUE) { // also followers still on their way in
            return 1;
        }
    }
    return 0;
}

void eies_sprite(DATAFILE *dat_file, enemyData *enem, unsigned int variant) {
    char file_buffer[14];
    // TODO usar variants
    int sheets[3] = {ENEMY1_BMP, ENEMY2_BMP, ENEMY3_BMP};
    int dead[3] = {ENEM1D_BMP, ENEM2D_BMP, ENEM3D_BMP};

    // load enemy1
    for (int i = 0; i < 9; i++) {
        enem->sprite[i] = dat_frame(sheets[variant - 1], 40 * i, 40, 40);
        enem->variant = variant;
        if (!enem->sprite[i]) {
            die("Cannot load enemy %d frame %d", variant, i);
        }
    }
    // load dead position
    //sprintf(file_buffer, "ENEM%dd.PCX", variant); 
    enem->sprite[11] = dat_file[dead[variant - 1]].dat;
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

void init_enemies(DATAFILE *dat_file) {
    for (int ec = 0; ec < MAX_ENEMIES; ec++) {
        eies_sprite(dat_file, &enemies[ec], ec % 3 + 1);
        alive_enemies[level][ec] = FALSE;
    }
    vespino_enemy.sprite[0] = dat_file[VESPINO2_BMP].dat;
    vespino_enemy.sprite[1] = dat_file[VESPINO3_BMP].dat;
}

void init_level_enemies(unsigned char prev_level) {
    // Enemies still standing when the player leaves through a side follow: they move to this
    // room for good and walk in from the side the player came from, one after another.
    char from_left = player.x < SCREEN_W / 2;
    char can_follow = prev_level >= 1 && prev_level < TOTAL_LEVELS && prev_level != level;
    for (int ec = 0; ec < MAX_ENEMIES; ec++) {
        if (can_follow && alive_enemies[prev_level][ec] == TRUE && enemies[ec].is_floor == FALSE
            && alive_enemies[level][ec] != TRUE) {
            alive_enemies[prev_level][ec] = ENEMY_NONE;
            alive_enemies[level][ec] = TRUE;
            enemies[ec].x = from_left ? levels[level].minX : levels[level].maxX - 20;
            enemies[ec].enter_delay = 50 + 30 * ec;
        } else {
            enemies[ec].x = levels[level].maxX - 60 - ec * 15; // it should vary per level
            enemies[ec].enter_delay = 0;
        }
        enemies[ec].y = 150 + ((ec % 2) * 3); // it should vary per enemy
        enemies[ec].targetX = 0;
        enemies[ec].targetY = 0;
        enemies[ec].waitTarget = 0;
        enemies[ec].curr_sprite = 0;
        enemies[ec].is_hit = FALSE;
        enemies[ec].enem_received_hit_sample = 0;
        enemies[ec].enem_hitted_sample = 0;
        if (alive_enemies[level][ec] == TRUE) {
            enemies[ec].is_floor = FALSE;
        } else {
            enemies[ec].is_floor = HIT_DURATION;
        }
        enemies[ec].is_punching = FALSE;
        enemies[ec].received_hits = 0;
        enemies[ec].knock = KNOCK_MAX;
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
int random_choice;

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
    if (x_distance <= 24 && y_distance <= LANE_REACH) {
        char hit = FALSE;
        // one strike per press, as in Target Renegade (holding the button used to hit again every
        // time the enemy's hit stun ended); the flying kick strikes while it is a kick
        char strike = player.jump > 0 || player.is_punching == 1 || player.is_kicking == 1;
        if (strike && (player.moving == PUNCH_LEFT || player.moving == KICK_LEFT) && enem->x <= player.x && !hitted_this_loop) {
            enem->enem_received_hit_sample = 1;
            score += 10;
            enem->is_hit = HIT_DURATION_ENEM;
            ++enem->received_hits;
            hitted_this_loop = TRUE;
            hit = TRUE;
        }
        if (strike && (player.moving == PUNCH_RIGHT || player.moving == KICK_RIGHT) && player.x <= enem->x && !hitted_this_loop) {
            score += 10;
            enem->enem_received_hit_sample = 1;
            enem->is_hit = HIT_DURATION_ENEM;
            ++enem->received_hits;
            hitted_this_loop = TRUE;
            hit = TRUE;
        }

        if (hit) {
            // being hit cancels the attack in progress (Target Renegade: the hit reaction replaces it)
            enem->is_punching = 0;
            enem->moving = player.x < enem->x ? STOP_LEFT : STOP_RIGHT;
        }
        if (enem->received_hits == 10) {
            // dead: only the kill sound (not the knockdown fall). The hit's punch sound is played
            // later this frame (game.c) and, longer and louder, would bury this short sample
            enem->enem_received_hit_sample = 0;
            play_sample(enemy_kill, 255, 127, 1000, 0);
            enem->is_floor = FLOOR_DURATION;
            enem->moving = MOVING_RIGHT;
            ++enem->floor_times;
            enem->received_hits = 0;
            return TRUE;
        }
        if (hit) {
            enem->knock -= KNOCK_HIT;
        }
        // a flying kick floors at once, a ground combo when the meter runs out; either way the enemy
        // gets up again (all_enemy_decisions), as in Target Renegade
        if (hit && (player.jump > 0 || enem->knock <= 0)) {
            enem->knock = KNOCK_MAX;
            stop_sample(punch);
            play_sample(fall, 255, 127, 1000, 0);
            enem->is_floor = FLOOR_DURATION;
            enem->moving = MOVING_RIGHT;
            ++enem->floor_times;
            return FALSE;
        }
    }

    // Target Renegade style: nothing is latched between frames except the attack in progress.
    // An attack is a fixed-length action that always ends; otherwise the "stick" is worked out
    // from scratch every frame: walk to the attack spot, line up with the player, then attack.
    enem->y_moving = STOPPOS;
    if (enem->punch_wait > 0) {
        enem->punch_wait--;
    }

    if (enem->is_punching > 0) {
        // the hit lands on one frame of the attack, so it can be dodged
        if (enem->is_punching == ATTACK_HIT_FRAME && player.is_floor == FALSE && y_distance <= LANE_REACH
            && x_distance <= FIGHT_DISTANCE
            && ((enem->moving == PUNCH_LEFT && player.x <= enem->x)
                || (enem->moving == PUNCH_RIGHT && player.x >= enem->x))) {
            player.is_hit = HIT_DURATION;
            player.curr_sprite = ANIM_HITTED;
            enem->enem_hitted_sample = 1;
            player.received_hits++;
            if (player.jump > 0) {
                // hit in the air: knocked down at once, the jump arc becomes the fall
                // (Target Renegade 58744: airborne actors skip the knockdown meter)
                player.received_hits = HIT_KO;
            }
            if (player.lifebar > 0 && cheat_mode != 1) {
                player.lifebar--;
            }
            draw_lifebar();
        }
        if (--enem->is_punching == 0) {
            enem->moving = enem->moving == PUNCH_LEFT ? STOP_LEFT : STOP_RIGHT;
        }
        return FALSE;
    }

    // targetX comes from assign_attack_spots() (0 = no spot: hold position)
    if (enem->targetX != FALSE && enem->targetX != enem->x) {
        if (enem->x > enem->targetX) {
            enem->x--;
            enem->moving = MOVING_LEFT;
        } else {
            enem->x++;
            enem->moving = MOVING_RIGHT;
        }
        return FALSE;
    }
    // half the player's speed on the lane, so the player can sidestep out of line (1px every other frame)
    if (enem->targetX != FALSE && enem->y != player.y && (counter & 1) == 0) {
        if (enem->y > player.y) {
            enem->y--;
            enem->y_moving = MOVING_UP;
        } else {
            enem->y++;
            enem->y_moving = MOVING_DOWN;
        }
    }

    // on the spot (or no spot): face the player
    char left = player.x < enem->x;
    enem->moving = left ? STOP_LEFT : STOP_RIGHT;
    if (y_distance <= LANE_REACH && player.is_floor == FALSE && player.is_hit == 0
        && x_distance <= FIGHT_DISTANCE && enem->punch_wait == 0 && random_choice > 10) {
        enem->moving = left ? PUNCH_LEFT : PUNCH_RIGHT;
        enem->curr_sprite = ANIM_PUNCH;
        enem->is_punching = ATTACK_FRAMES;
        enem->punch_wait = ATTACK_FRAMES + ATTACK_COOLDOWN;
    }
    return FALSE;
}

inline void draw_enemy(int index) {
    if (!enemy_in_room(index)) {
        return;
    }


    if (enemies[index].is_floor != FALSE) {
        if (enemies[index].moving & 1) {
            draw_sprite(double_buffer, enemies[index].sprite[11],
                        enemies[index].x, enemies[index].y + 30);
        } else {
            draw_sprite_h_flip(double_buffer, enemies[index].sprite[11],
                               enemies[index].x,
                               enemies[index].y + 30);
        }

    } else {
        if (!enemies[index].sprite[enemies[index].curr_sprite]) {
            return;
        }
        // redraw pair or impair?
        if (enemies[index].moving & 1) {
            draw_sprite_h_flip(
                double_buffer, enemies[index].sprite[enemies[index].curr_sprite],
                enemies[index].x, enemies[index].y);
        } else {
            draw_sprite(double_buffer,
                        enemies[index].sprite[enemies[index].curr_sprite],
                        enemies[index].x, enemies[index].y);
        }
    }
}

void all_enemy_animations() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_in_room(i)) {
            enemy_animation(&enemies[i]);
        }
    }
}

void clean_vespino() {
    blit(bg, double_buffer, vespino_enemy.x - 3, vespino_enemy.y,
         vespino_enemy.x - 3, vespino_enemy.y, 55, 50);
}

void enem_resets() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
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

// Target Renegade's AI: attack spots at both sides of the player, and the cheapest
// enemy/spot pair is taken first, so enemies flank the player instead of piling up.
// Inner spots (FIGHT_DISTANCE) first, the remaining enemies wait at the outer ones.
static void assign_attack_spots() {
    int n = MAX_ENEMIES;
    char free_enem[MAX_ENEMIES];
    for (int e = 0; e < n; e++) {
        free_enem[e] = enemy_in_room(e) && enemies[e].is_floor == FALSE;
        enemies[e].targetX = 0; // recomputed every frame
    }
    for (int ring = 1; ring <= 2; ring++) {
        int spot[2] = {(int)player.x - ring * FIGHT_DISTANCE, (int)player.x + ring * FIGHT_DISTANCE};
        // spots outside the level are not used (> minX also keeps targetX != FALSE)
        char free_spot[2] = {spot[0] > (int)levels[level].minX, spot[1] < (int)levels[level].maxX};
        for (int k = 0; k < 2; k++) {
            int best = 0x7fff, bs = 0, be = 0;
            for (int s = 0; s < 2; s++) {
                if (!free_spot[s]) {
                    continue;
                }
                for (int e = 0; e < n; e++) {
                    if (!free_enem[e]) {
                        continue;
                    }
                    // horizontal distance weighs 4x, as in the original
                    int cost = 4 * abs((int)enemies[e].x - spot[s]) + abs((int)enemies[e].y - (int)player.y);
                    if (cost < best) {
                        best = cost;
                        bs = s;
                        be = e;
                    }
                }
            }
            if (best == 0x7fff) {
                break;
            }
            enemies[be].targetX = spot[bs];
            free_spot[bs] = FALSE;
            free_enem[be] = FALSE;
        }
    }
}

void all_enemy_decisions() {

    hitted_this_loop = FALSE;
    assign_attack_spots();

    alive_enemies_count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        random_choice = rand() & 0b111111;
        if (alive_enemies[level][i] == TRUE) {
            ++alive_enemies_count;
        }
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].enter_delay > 0) {
            enemies[i].enter_delay--; // follower still on its way in
            continue;
        }
        if (!enemy_in_room(i)) {
            continue;
        }
        if (enemies[i].knock < KNOCK_MAX) {
            enemies[i].knock++;
        }
        // floored but not dead (flying kick): gets up after FLOOR_DURATION ticks, like the player
        if (enemies[i].is_floor > 0 && alive_enemies[level][i] == TRUE && small_counter == 10
            && --enemies[i].is_floor == FALSE) {
            enemies[i].targetX = 0;
            enemies[i].targetY = 0;
            enemies[i].is_punching = 0;
        }
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

            if (x_distance < 20 && x_distance >= 8 && y_distance < 8) {
                if ((player.moving == PUNCH_LEFT || player.moving == KICK_LEFT) && player.x > vespino_enemy.x) {
                    vespino_hitted();                   
                } else  if ((player.moving == PUNCH_RIGHT || player.moving == KICK_RIGHT) && player.x < vespino_enemy.x) {
                    vespino_hitted();                    
                }
                if (vespino_enemy.lifebar == 0) {
                    player.win = TRUE;
                    next_level = 0;
                    return;
                }
            }
            
            if (x_distance < 6 && y_distance < 8) {
                player.is_floor = FLOOR_DURATION / 2;
                player.received_hits = MOTORBIKE_HIT;
                if (cheat_mode != 1) {
                    if (player.lifebar < 2) {
                        player.lifebar = 0;
                    } else {
                        player.lifebar -= 2;
                    }
                    draw_lifebar();
                }
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

    if (vespino_enemy.offset == 0) {
        vespino_enemy.offset = 1;
    } else {
        vespino_enemy.offset = 0;
    }
    if (!vespino_enemy.sprite[vespino_enemy.offset]) {
        return;
    }
    if (vespino_enemy.direction == VESPINO_LEFT) {
        draw_sprite_h_flip(screen, vespino_enemy.sprite[vespino_enemy.offset], vespino_enemy.x, vespino_enemy.y);
    } else if (vespino_enemy.direction == VESPINO_RIGHT) {
        draw_sprite(screen, vespino_enemy.sprite[vespino_enemy.offset], vespino_enemy.x, vespino_enemy.y);
    }
}


void redraw_bg_enemy_positions() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_in_room(i)) blit(bg, double_buffer, enemies[i].x, 120,
             enemies[i].x, 120, 40, 80);
    }
    if (vespino_enemy.direction != VESPINO_HIDDEN) {
        clean_vespino();
    }
}


inline int enemy_on_path(unsigned int new_player_x) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_in_room(i) || enemies[i].is_floor != FALSE) {
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