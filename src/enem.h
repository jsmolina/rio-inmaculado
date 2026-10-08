#ifndef ENEM_H
#define ENEM_H
#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>

#define MAX_ENEMIES 3
#define JOHNY 1
#define PETER 2
#define ALEX 3
#define VESPINO_HIDDEN 0
#define VESPINO_LEFT 1
#define VESPINO_RIGHT 2
#define VESPINO_SPEED 3

#define FIGHT_DISTANCE 22
// enemy attack: fixed-length action (frames), the hit is checked once, ATTACK_HIT_FRAME frames
// before the end; then ATTACK_COOLDOWN frames before the next one
#define ATTACK_FRAMES 22
#define ATTACK_HIT_FRAME 16
#define ATTACK_COOLDOWN 35
// hits (both ways) only connect between actors in the same lane: |dy| <= LANE_REACH
#define LANE_REACH 2
// Target Renegade knockdown meter (58744): each hit drains KNOCK_HIT, it refills 1 per frame up to
// KNOCK_MAX; when it runs out the enemy is floored for a while and gets up (alive). Only a quick
// combo (3 hits) floors; life is still the 10 hits that kill.
#define KNOCK_MAX 40
#define KNOCK_HIT 24
// alive_enemies[room][slot]: TRUE alive, FALSE dead (body on the floor), ENEMY_NONE nobody
// (the room has no enemy in that slot, or it followed the player to another room)
#define ENEMY_NONE 2

#define JOHNY_INDEX 0
#define PETER_INDEX 1
#define ALEX_INDEX 2
#define PLAYER_INDEX 3
#define VESPINO_INDEX 4

extern enemyData enemies[MAX_ENEMIES];
extern int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];
extern vespinoData vespino_enemy;

// initializes enemies on level
void init_level_enemies(unsigned char prev_level);
// TRUE if slot i has an enemy (standing or lying) in the current room right now
int enemy_in_room(int i);
void init_enemies(DATAFILE *dat_file);

// Animations for all enemies
void all_enemy_animations();

// AI for all enemies
void all_enemy_decisions();
void enem_resets();

// draw all enemies
inline void draw_enemy(int index);
void draw_vespino();

// true if has any enemy on path
inline int enemy_on_path(unsigned int new_player_x);
// redraws background over all the enemy positions
void redraw_bg_enemy_positions();
// frees up memory sprites for enemies
void unload_enemies();

// returns TRUE if has active enemies in current level
int has_alive_enemies();

#endif