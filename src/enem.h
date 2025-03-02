#ifndef ENEM_H
#define ENEM_H
#include "helpers.h"
#include <allegro.h>
#include <allegro/gfx.h>

#define MAX_ENEMIES 3
#define JOHNY 1
#define PETER 2
#define ALEX 3

#define FIGHT_DISTANCE 20

extern enemyData enemies[MAX_ENEMIES];
extern int alive_enemies[TOTAL_LEVELS][MAX_ENEMIES];

// initializes enemies on level
void init_level_enemies(int maxX, char first_load);
// Animations for all enemies
void all_enemy_animations();
// AI for all enemies
void all_enemy_decisions(spritePos * playr);
// draw all enemies
void all_draw_enemies();
// returns true if player has Y upper than all enemies
char player_over_all_enemies(int player_y);
// true if has any enemy on path
char enemy_on_path(int new_player_x, int play_y);
// redraws background over all the enemy positions
void redraw_bg_enemy_positions();
// frees up memory sprites for enemies
void unload_enemies();

#endif