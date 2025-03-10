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

#define FIGHT_DISTANCE 20

extern enemyData enemies[MAX_ENEMIES];
extern int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];
extern vespinoData vespino_enemy;

// initializes enemies on level
void init_level_enemies();
void init_enemies();
// Animations for all enemies
void all_enemy_animations();
// AI for all enemies
void all_enemy_decisions(spritePos * playr);
// draw all enemies
void all_draw_enemies();
// returns true if player has Y upper than all enemies
int player_over_all_enemies();
// true if has any enemy on path
int enemy_on_path(unsigned int new_player_x);
// redraws background over all the enemy positions
void redraw_bg_enemy_positions();
// frees up memory sprites for enemies
void unload_enemies();

// returns TRUE if has active enemies in current level
int has_alive_enemies();

#endif